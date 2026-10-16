from pathlib import Path
p=Path('linux/drivers/misc/idma.c');s=p.read_text()
s=s.replace('/* Reserved launcher lane IDs: V is lane 0 (ID 1), A is lane 1 (ID 2). */\n#define IDMA_CONTROLLED_V_ID 1U\n#define IDMA_CONTROLLED_A_ID 2U\n','/* Shared AXI identity of the A read selected by the IOMMU. */\n')
s=s.replace('(a && e->aggressor_pages != 1)', 'e->aggressor_pages != 1')
a=s.index('static int idma_controlled_environment(');b=s.index('static int idma_controlled_prep_task(',a)
t=s[a:b].replace('/* All idle and owned: disable', '''if (readq(victim->reg + IDMA_STATUS_REG_OFFSET) ||
	    (aggressor && readq(aggressor->reg + IDMA_STATUS_REG_OFFSET)))
		return -EBUSY;
	/* All idle and owned: disable''');s=s[:a]+t+s[b:]
a=s.index('static int idma_controlled_prep_task(');b=s.index('/*\n * Accounting only:',a)
t=s[a:b].replace('memset(stats, 0, sizeof(*stats));','''if (s->in_flight || s->retain)
		return -EBUSY;
	if (ktime_get_ns() >= deadline)
		return -ETIMEDOUT;
	if (signal_pending(current))
		return -EINTR;
	memset(stats, 0, sizeof(*stats));''',1).replace('ret = idma_submit(s, false);\n\tif (!ret)\n\t\tret = idma_drain(s);','''ret = idma_submit(s, false);
	if (!ret) {
		stats->submitted = 1;
		ret = idma_drain(s);
	}''').replace('\tstats->submitted = ret ? 0 : 1;\n','').replace('A no-op probe must be a real completion with a cold IOTLB.','The probe must actually copy while both V translations are hot.');s=s[:a]+t+s[b:]
a=s.index('static int idma_controlled_account(');b=s.index('\n/*\n * Only called',a)
s=s[:a]+'''static int idma_controlled_account(struct idma_stream *v, struct idma_stream *a,
				   u8 reserved_mask, u64 *words)
{
	u64 expected = a ? 3 : 1;
	u64 issued = words[15], done = words[16], cancel = words[17];
	unsigned int lane;

	if (words[0] != IDMA_LAUNCHER_MAGIC || !idma_launcher_terminal(words[2]) ||
	    words[19] != expected || words[28] != !!a || words[29] || words[30] ||
	    words[31] || (reserved_mask & ~expected) || issued != done ||
	    (done & cancel) || (done | cancel) != reserved_mask ||
	    words[18] != reserved_mask)
		return -EIO;
	for (lane = 0; lane < 2; lane++) {
		struct idma_stream *stream = lane ? a : v;
		u64 n = !!(issued & BIT(lane));

		if (words[24 + lane] != n || words[26 + lane] != n)
			return -EIO;
		if (n) {
			if (!stream || !stream->transfer_id ||
			    words[11 + lane] != stream->transfer_id ||
			    words[13 + lane] != stream->transfer_id)
				return -EIO;
		} else if (words[11 + lane] || words[13 + lane]) {
			return -EIO;
		}
	}
	return 0;
}
''' + s[b:]
a=s.index('static int idma_controlled_validate_success(');b=s.index('\n/*\n * Single conservative',a)
s=s[:a]+'''static int idma_controlled_validate_success(struct idma_stream *v,
					    struct idma_stream *a, u64 *words)
{
	u8 expected = a ? 3 : 1;

	if (idma_controlled_account(v, a, expected, words) || words[1] ||
	    (words[2] & IDMA_LAUNCHER_STATUS_ABORTED) ||
	    words[15] != expected || words[16] != expected || words[17])
		return -EIO;
	if (!words[6] || words[6] > words[7] || words[7] > words[9])
		return -EIO;
	if (a) {
		if (!words[4] || words[4] > words[8] || words[8] > words[5] ||
		    words[5] >= words[6] || words[10] < words[8] ||
		    words[20] != IDMA_CONTROLLED_A_SELECT_ID ||
		    words[21] != IDMA_CONTROLLED_A_SELECT_DID ||
		    words[22] != a->src_iova || words[23])
			return -EIO;
	} else if (words[4] || words[5] || words[8] || words[10] ||
		   words[20] || words[21] || words[22] || words[23]) {
		return -EIO;
	}
	return 0;
}
''' + s[b:]
a=s.index('static int idma_controlled_cleanup(');b=s.index('\n/*\n * Formal round:',a)
t=s[a:b].replace('int ret2;', 'int ret2;\n\tbool terminal;').replace('''			goto quarantine;
		}
	}

	ret2 = idma_launcher_read_words''','''		}
	}
	terminal = idma_launcher_terminal(status);

	/* Preserve live raw evidence even if terminal proof timed out. */
	ret2 = idma_launcher_read_words''',1).replace('if (ret2) {\n\t\tif (!ret)', 'if (ret2 || !terminal) {\n\t\tif (!ret)',1).replace('ret = ret2;\n\t\tgoto quarantine;', 'ret = ret2 ? ret2 : -ETIMEDOUT;\n\t\tgoto quarantine;',1).replace('idma_controlled_account(a, reserved_mask, words)','idma_controlled_account(v, a, reserved_mask, words)').replace('idma_controlled_validate_success(v, a, e, words)','idma_controlled_validate_success(v, a, words)')
s=s[:a]+t+s[b:]
a=s.index('static int idma_controlled_formal(');b=s.index('\nstatic int idma_controlled_one_round(',a)
t=s[a:b].replace('u64 status, arm_stamp, start, end, v_done = 0;', 'u64 status, arm_stamp, start = 0, end, drain_start, v_done = 0;').replace('int ret, poll_ret;', 'int ret, poll_ret;\n\tbool started = false;')
# All formal exits funnel through owned cleanup, with snapshot only if started.
t=t.replace('return -EIO;', '{ ret = -EIO; goto out; }').replace('return ret;', 'goto out;')
t=t.replace('e->measurement_started = 1;', 'e->measurement_started = 1;\n\tstarted = true;\n\te->in_flight_at_start = v->in_flight + (a && a->in_flight);')
t=t.replace('if (idma_launcher_terminal(status))\n\t\t\tbreak;', '''/* Terminal can become visible after the earlier V DONE poll.
		 * Keep polling until that completion has also reached the observer. */
		if (idma_launcher_terminal(status) && !v->in_flight &&
		    (!a || !a->in_flight))
			break;''')
start=t.index('\n\tend = ktime_get_ns();',t.index('for (;;)'))
t=t[:start]+'''
out:
	drain_start = ktime_get_ns();
	if (!ret)
		round->stage = IDMA_CONTROLLED_STAGE_DRAIN;
	ret = idma_controlled_cleanup(launcher, v, a, e, round,
				      *reserved_mask, *owned, ret);
	*owned = false;
	e->drain_ns = ktime_get_ns() - drain_start;
	if (started) {
		if (v_done)
			e->victim_elapsed_ns = v_done - start;
		end = ktime_get_ns();
		idma_snapshot(hpm, v, a, &after, active_after);
		e->snapshot_after_ns = ktime_get_ns() - end;
		idma_snapshot_delta(&e->victim, 0, &before, &after,
				    active_before[0], active_after[0]);
		if (a)
			idma_snapshot_delta(&e->aggressor, 1, &before, &after,
					    active_before[1], active_after[1]);
	}
	return ret;
}
'''
s=s[:a]+t+s[b:]
a=s.index('static int idma_controlled_one_round(');b=s.index('\nstatic long idma_run_e3_controlled(',a)
t=s[a:b];i=t.index('\n\t{\n\t\tint first = ret;');j=t.index('\n\n\tif (!ret) {',i);t=t[:i]+t[j:]
t=t.replace('''	if (!ret)
		round->stage = IDMA_CONTROLLED_STAGE_COMPLETE;
	e->status = ret;''','''	e->victim.quarantined = v->retain;
	if (a)
		e->aggressor.quarantined = a->retain;
	e->in_flight_at_end = v->in_flight + (a && a->in_flight);
	if (!ret) {
		round->stage = IDMA_CONTROLLED_STAGE_COMPLETE;
		e->measurement_complete = 1;
	}
	e->status = ret;''')
s=s[:a]+t+s[b:];p.write_text(s)
