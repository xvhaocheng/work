"""Independent, untimed LR1 ordinary control policies.

The frozen snapshot contains results of *paid, public* target primitives.
None of these policies discovers readers, measures real R acceptance, seals
hardware, or performs PTE/cache/CQ operations.  The common adapter must do so.
Only ``ack`` authorizes successful page release; ``error`` quarantines it.
Extra snapshot fields are ignored and cannot provide a private oracle.
"""

from collections.abc import Mapping


BOOL_FIELDS = (
    "readonly",
    "nonoverlap",
    "registry_complete",
    "alias_safe",
    "all_sources_sealed",
    "all_readers_complete",
    "read_error",
    "authorization_revoked",
    "cache_synced",
    "invalidation_committed",
    "target_ordering_proven",
    "whole_copy_complete",
    "request_active",
)
INT_FIELDS = ("request_epoch", "current_epoch", "old_obligations")
SNAPSHOT_FIELDS = BOOL_FIELDS + INT_FIELDS
ACTIONS = frozenset(("wait", "seal", "revoke", "sync", "invalidate", "order", "ack", "error"))


def _validated(snapshot):
    """Capture once and fail closed on absent, nonliteral or invalid facts."""
    if not isinstance(snapshot, Mapping):
        return None
    try:
        facts = {name: snapshot[name] for name in SNAPSHOT_FIELDS}
    except (KeyError, TypeError):
        return None
    if any(type(facts[name]) is not bool for name in BOOL_FIELDS):
        return None
    if any(type(facts[name]) is not int or facts[name] < 0 for name in INT_FIELDS):
        return None
    return facts


def whole_copy(snapshot):
    """Ordinary job completion followed by source unmap and release.

    Success waits for whole-copy completion before starting source retirement.
    Error may stop/drain earlier because a failed whole copy may never complete.
    """
    facts = _validated(snapshot)
    if facts is None or not facts["request_active"]:
        return "wait"
    if facts["request_epoch"] != facts["current_epoch"]:
        return "wait"
    if facts["read_error"]:
        if not facts["all_sources_sealed"]:
            return "seal"
        if facts["old_obligations"] != 0:
            return "wait"
        return "error"
    if not all(facts[name] for name in ("readonly", "nonoverlap", "registry_complete", "alias_safe")):
        return "wait"
    if not facts["whole_copy_complete"]:
        return "wait"
    if not facts["all_sources_sealed"]:
        return "seal"
    if not facts["all_readers_complete"] or facts["old_obligations"] != 0:
        return "wait"
    if not facts["authorization_revoked"]:
        return "revoke"
    if not facts["cache_synced"]:
        return "sync"
    if not facts["invalidation_committed"]:
        return "invalidate"
    if not facts["target_ordering_proven"]:
        return "order"
    return "ack"


def phased(snapshot):
    """Ordinary source-phase join followed by target-only unmap.

    The join is exact real-R receipt for every registered old-generation reader,
    plus retirement of all raw/walk/helper/OUT/held/accepted obligations.
    It can finish while nonoverlapping destination W/B continues.
    """
    facts = _validated(snapshot)
    if facts is None or not facts["request_active"]:
        return "wait"
    if facts["request_epoch"] != facts["current_epoch"]:
        return "wait"
    if facts["read_error"]:
        if not facts["all_sources_sealed"]:
            return "seal"
        if facts["old_obligations"] != 0:
            return "wait"
        return "error"
    if not facts["readonly"] or not facts["nonoverlap"]:
        return "wait"
    if not facts["registry_complete"] or not facts["alias_safe"]:
        return "wait"
    if not facts["all_sources_sealed"]:
        return "seal"
    if facts["old_obligations"] != 0 or not facts["all_readers_complete"]:
        return "wait"
    if not facts["authorization_revoked"]:
        return "revoke"
    if not facts["cache_synced"]:
        return "sync"
    if not facts["invalidation_committed"]:
        return "invalidate"
    if not facts["target_ordering_proven"]:
        return "order"
    return "ack"


def targeted_drain(snapshot):
    """Independent driver-controlled scoped-invalidation/drain sequence.

    This is a protocol policy, not a VT-d implementation on the RISC-V FPGA.
    VT-d's page/domain guarantee covers already remapped untranslated memory
    requests; it does not furnish this policy's DMA real-R receipt or raw gate.
    A common adapter must independently supply and pay for those stronger facts.
    """
    facts = _validated(snapshot)
    if facts is None or not facts["request_active"]:
        return "wait"
    if facts["request_epoch"] != facts["current_epoch"]:
        return "wait"
    if facts["read_error"]:
        if not facts["all_sources_sealed"]:
            return "seal"
        if facts["old_obligations"] != 0:
            return "wait"
        return "error"
    qualified = (
        facts["readonly"]
        and facts["nonoverlap"]
        and facts["registry_complete"]
        and facts["alias_safe"]
    )
    if not qualified:
        return "wait"
    if not facts["all_sources_sealed"]:
        return "seal"
    if not facts["all_readers_complete"]:
        return "wait"
    if facts["old_obligations"] != 0:
        return "wait"
    if not facts["authorization_revoked"]:
        return "revoke"
    if not facts["cache_synced"]:
        return "sync"
    if not facts["invalidation_committed"]:
        return "invalidate"
    if not facts["target_ordering_proven"]:
        return "order"
    return "ack"
