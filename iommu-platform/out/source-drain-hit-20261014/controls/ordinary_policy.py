"""O-NBO: committed-write-owner scheduling on an ordinary hit-under-miss lane.

This module is independently executable and imports no candidate, model or oracle.
`choose` only proposes a pre-VALID selection. The caller retains the original AR
cell, freezes a published offer, rechecks the paid guard on the real handshake,
and implements response/ID/epoch lifetimes. See ordinary-program.md.
"""

from __future__ import annotations

import json
import sys
from typing import Any


def _identity_integer(value: Any) -> bool:
    return isinstance(value, int) and not isinstance(value, bool) and value >= 0


def choose(state: dict[str, Any]) -> tuple[int, int, int] | None:
    """Select the current committed W producer, if its oldest AR is a legal hit.

    Required Boolean fields are interpreted strictly: absent qualification never
    grants an action. `drain.source/job` identifies both the current W head and
    its accepted physical AW. An adapter with separate AW identity fields must
    provide both `aw_source` and `aw_job`; this function checks them too.
    """
    if not isinstance(state, dict):
        return None
    if not (
        state.get("ptw_busy") is True
        and state.get("result_free") is True
        and state.get("normal_or_invalidation_pending") is False
        and state.get("grant_used") is False
    ):
        return None

    active = state.get("active_source")
    if not _identity_integer(active):
        return None
    drain = state.get("drain")
    if not isinstance(drain, dict):
        return None
    if not (
        drain.get("aw_accepted") is True
        and drain.get("preceding_independent") is True
        and _identity_integer(drain.get("source"))
        and "job" in drain
        and drain["job"] is not None
    ):
        return None
    owner = drain["source"]
    if owner == active:
        return None
    if "aw_source" in drain or "aw_job" in drain:
        if drain.get("aw_source") != owner or drain.get("aw_job") != drain["job"]:
            return None

    heads = state.get("heads")
    if not isinstance(heads, list):
        return None
    owner_heads = [
        head
        for head in heads
        if isinstance(head, dict)
        and head.get("source") == owner
        and head.get("oldest") is True
    ]
    # A snapshot with two oldest cells for one source has no unambiguous owner.
    if len(owner_heads) != 1:
        return None
    head = owner_heads[0]
    if not (
        head.get("job") == drain["job"]
        and head.get("hit") is True
        and head.get("permissions_ok") is True
        and head.get("native_guard_ok") is True
        and head.get("needed_for_head") is True
        and _identity_integer(head.get("source"))
        and _identity_integer(head.get("slot"))
        and _identity_integer(head.get("generation"))
    ):
        return None
    return head["source"], head["slot"], head["generation"]


if __name__ == "__main__":
    # One JSON object, or a list of independent objects, from stdin; tuple->array.
    payload = json.load(sys.stdin)
    result = [choose(item) for item in payload] if isinstance(payload, list) else choose(payload)
    json.dump(result, sys.stdout, ensure_ascii=False)
    sys.stdout.write("\n")
