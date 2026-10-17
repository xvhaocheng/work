## Material Passport

ARS-Codex experiment-agent / validate; DISCOVERY-REUSE-1; independent raw-output reducer. No simulation was executed by this reducer.

Observed attempts: 8. Fully valid: True.

| Case / attempt | valid | T_V | T_A | T_data | T_return |
|---|---:|---:|---:|---:|---:|
| base-p-r1 / attempt01 | True | 76300 | 75726 | 76300 | 76331 |
| base-p-r16 / attempt01 | True | 101305 | 100793 | 101305 | 101334 |
| base-p-r8 / attempt01 | True | 90261 | 89749 | 90261 | 90290 |
| base-pn-r1 / attempt01 | True | 76843 | 76269 | 76843 | 76869 |
| base-pn-r16 / attempt01 | True | 130078 | 129566 | 130078 | 130104 |
| base-pn-r8 / attempt01 | True | 106542 | 106030 | 106542 | 106570 |
| diag-p-r16-2m / attempt01 | True | 75770 | 75196 | 75770 | 75797 |
| diag-pn-r16-2m / attempt01 | True | 75770 | 75196 | 75770 | 75797 |

### runs/base-p-r1/attempt01


Translation observations:

- Physical first lookup checks / N probes: {"normal_hit": 508, "normal_miss": 4}.
- Unique accepted logical requests: {"n_accepted": 0, "normal_accepted": 512, "normal_first_check_hit": 508, "normal_first_check_miss": 4, "normal_with_walk": 4}.
- Unique PTW sequences: 4; grouped by actual owner/page: {"lane0_destination_all": 1, "lane0_source_all": 1, "lane1_destination_all": 1, "lane1_source_all": 1}.

Behavioral DDR model truth (total / min / max across read occurrences):

- queued_cycles_before_head: 160088 / 0 / 798
- startup_ticks: 8370 / 31 / 31
- backpressure_ticks: 208 / 0 / 104
- valid_ready_ticks: 65564 / 1 / 256
- other_head_ticks: 0 / 0 / 0

Queue distance is separately retained as raw acceptance-to-head edge distance. Startup classification uses the explicit setup-wait flag, including the final decrement-to-zero tick. These are model observations, not controller-visible information.

Supply and overlap remain separate: {"both_lanes_issue_to_done": 71764, "either_lane_issue_to_done": 76258, "interpretation": "descriptive interval intersection, not recoverable or saved time"}.

AW flags can coexist; no unique root-cause precedence is assigned: {"by_lane": {"0": {"each_flag_cycles_not_additive": {"0": 36544, "1": 0, "2": 0, "3": 0}, "stall_cycles": 36544}, "1": {"each_flag_cycles_not_additive": {"0": 33456, "1": 0, "2": 0, "3": 0}, "stall_cycles": 33456}}, "flag_semantics": ["open write selection conflict", "ID/count capacity", "occupied AW ID mapped to another selection", "selected downstream AW valid and not ready"], "stall_cycles_union": 70000}.

Lane 0 stage cycles (total / min / max):

- ready_to_issue: 1999 / 26 / 47
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 36119 / 67 / 1957
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 3110 / 7 / 1921
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 586 / 4 / 20
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 5 / 0 / 5
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 575 / 4 / 14

Lane 1 stage cycles (total / min / max):

- ready_to_issue: 2005 / 26 / 42
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 37205 / 127 / 2921
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 1444 / 7 / 442
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 577 / 0 / 14
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 20 / 0 / 20
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 584 / 4 / 14

Fixed example last_finishing_parent: lane 0, j 63, parent CSV line 65; ready 75152, issue 75186, backend 75188, first local R 75747, all source 76289, last B 76296, DONE 76300.

Fixed example longest_latency_V_tie_lowest_j: lane 0, j 0, parent CSV line 2; ready 0, issue 47, backend 49, first local R 2006, all source 2548, last B 4469, DONE 4473.

### runs/base-p-r16/attempt01


Translation observations:

- Physical first lookup checks / N probes: {"normal_hit": 445, "normal_miss": 67}.
- Unique accepted logical requests: {"n_accepted": 0, "normal_accepted": 512, "normal_first_check_hit": 445, "normal_first_check_miss": 67, "normal_with_walk": 67}.
- Unique PTW sequences: 67; grouped by actual owner/page: {"lane0_destination_all": 1, "lane0_source_all": 1, "lane1_destination_after_ringlap": 48, "lane1_destination_all": 64, "lane1_destination_repeated_page": 48, "lane1_source_all": 1}.

Behavioral DDR model truth (total / min / max across read occurrences):

- queued_cycles_before_head: 43438 / 0 / 508
- startup_ticks: 14229 / 31 / 31
- backpressure_ticks: 208 / 0 / 104
- valid_ready_ticks: 65753 / 1 / 256
- other_head_ticks: 0 / 0 / 0

Queue distance is separately retained as raw acceptance-to-head edge distance. Startup classification uses the explicit setup-wait flag, including the final decrement-to-zero tick. These are model observations, not controller-visible information.

Supply and overlap remain separate: {"both_lanes_issue_to_done": 96853, "either_lane_issue_to_done": 101263, "interpretation": "descriptive interval intersection, not recoverable or saved time"}.

AW flags can coexist; no unique root-cause precedence is assigned: {"by_lane": {"0": {"each_flag_cycles_not_additive": {"0": 64387, "1": 0, "2": 0, "3": 0}, "stall_cycles": 64387}, "1": {"each_flag_cycles_not_additive": {"0": 29051, "1": 0, "2": 0, "3": 0}, "stall_cycles": 29051}}, "flag_semantics": ["open write selection conflict", "ID/count capacity", "occupied AW ID mapped to another selection", "selected downstream AW valid and not ready"], "stall_cycles_union": 93438}.

Lane 0 stage cycles (total / min / max):

- ready_to_issue: 1993 / 26 / 47
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 34473 / 457 / 1957
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 29767 / 442 / 1921
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 580 / 4 / 20
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 5 / 0 / 5
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 567 / 4 / 14

Lane 1 stage cycles (total / min / max):

- ready_to_issue: 1989 / 26 / 42
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 7403 / 60 / 2921
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 56329 / 7 / 894
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 561 / 0 / 14
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 20 / 0 / 20
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 570 / 4 / 14

Fixed example last_finishing_parent: lane 0, j 63, parent CSV line 65; ready 99767, issue 99793, backend 99795, first local R 100317, all source 100859, last B 101301, DONE 101305.

Fixed example longest_latency_V_tie_lowest_j: lane 0, j 0, parent CSV line 2; ready 0, issue 47, backend 49, first local R 2006, all source 2548, last B 4469, DONE 4473.

### runs/base-p-r8/attempt01


Translation observations:

- Physical first lookup checks / N probes: {"normal_hit": 473, "normal_miss": 39}.
- Unique accepted logical requests: {"n_accepted": 0, "normal_accepted": 512, "normal_first_check_hit": 473, "normal_first_check_miss": 39, "normal_with_walk": 39}.
- Unique PTW sequences: 39; grouped by actual owner/page: {"lane0_destination_all": 1, "lane0_source_all": 1, "lane1_destination_after_ringlap": 28, "lane1_destination_all": 36, "lane1_destination_repeated_page": 28, "lane1_source_all": 1}.

Behavioral DDR model truth (total / min / max across read occurrences):

- queued_cycles_before_head: 55548 / 0 / 508
- startup_ticks: 11625 / 31 / 31
- backpressure_ticks: 208 / 0 / 104
- valid_ready_ticks: 65669 / 1 / 256
- other_head_ticks: 0 / 0 / 0

Queue distance is separately retained as raw acceptance-to-head edge distance. Startup classification uses the explicit setup-wait flag, including the final decrement-to-zero tick. These are model observations, not controller-visible information.

Supply and overlap remain separate: {"both_lanes_issue_to_done": 85809, "either_lane_issue_to_done": 90219, "interpretation": "descriptive interval intersection, not recoverable or saved time"}.

AW flags can coexist; no unique root-cause precedence is assigned: {"by_lane": {"0": {"each_flag_cycles_not_additive": {"0": 53343, "1": 0, "2": 0, "3": 0}, "stall_cycles": 53343}, "1": {"each_flag_cycles_not_additive": {"0": 29051, "1": 0, "2": 0, "3": 0}, "stall_cycles": 29051}}, "flag_semantics": ["open write selection conflict", "ID/count capacity", "occupied AW ID mapped to another selection", "selected downstream AW valid and not ready"], "stall_cycles_union": 82394}.

Lane 0 stage cycles (total / min / max):

- ready_to_issue: 1993 / 26 / 47
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 26901 / 122 / 1957
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 26295 / 318 / 1921
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 580 / 4 / 20
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 5 / 0 / 5
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 567 / 4 / 14

Lane 1 stage cycles (total / min / max):

- ready_to_issue: 1989 / 26 / 42
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 10751 / 60 / 2921
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 41937 / 7 / 894
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 561 / 0 / 14
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 20 / 0 / 20
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 570 / 4 / 14

Fixed example last_finishing_parent: lane 0, j 63, parent CSV line 65; ready 89237, issue 89266, backend 89268, first local R 89397, all source 89939, last B 90257, DONE 90261.

Fixed example longest_latency_V_tie_lowest_j: lane 0, j 0, parent CSV line 2; ready 0, issue 47, backend 49, first local R 2006, all source 2548, last B 4469, DONE 4473.

### runs/base-pn-r1/attempt01


Translation observations:

- Physical first lookup checks / N probes: {"n_probe_hit": 4, "n_probe_miss": 1, "normal_hit": 504, "normal_miss": 4}.
- Unique accepted logical requests: {"n_accepted": 4, "normal_accepted": 508, "normal_first_check_hit": 504, "normal_first_check_miss": 4, "normal_with_walk": 4}.
- Unique PTW sequences: 4; grouped by actual owner/page: {"lane0_destination_all": 1, "lane0_source_all": 1, "lane1_destination_all": 1, "lane1_source_all": 1}.

Behavioral DDR model truth (total / min / max across read occurrences):

- queued_cycles_before_head: 160326 / 0 / 798
- startup_ticks: 8370 / 31 / 31
- backpressure_ticks: 208 / 0 / 104
- valid_ready_ticks: 65564 / 1 / 256
- other_head_ticks: 0 / 0 / 0

Queue distance is separately retained as raw acceptance-to-head edge distance. Startup classification uses the explicit setup-wait flag, including the final decrement-to-zero tick. These are model observations, not controller-visible information.

Supply and overlap remain separate: {"both_lanes_issue_to_done": 72325, "either_lane_issue_to_done": 76801, "interpretation": "descriptive interval intersection, not recoverable or saved time"}.

AW flags can coexist; no unique root-cause precedence is assigned: {"by_lane": {"0": {"each_flag_cycles_not_additive": {"0": 36528, "1": 0, "2": 0, "3": 0}, "stall_cycles": 36528}, "1": {"each_flag_cycles_not_additive": {"0": 34020, "1": 0, "2": 0, "3": 0}, "stall_cycles": 34020}}, "flag_semantics": ["open write selection conflict", "ID/count capacity", "occupied AW ID mapped to another selection", "selected downstream AW valid and not ready"], "stall_cycles_union": 70548}.

Lane 0 stage cycles (total / min / max):

- ready_to_issue: 1994 / 26 / 47
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 36019 / 49 / 1957
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 3758 / 7 / 2485
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 581 / 4 / 20
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 5 / 0 / 5
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 565 / 4 / 14

Lane 1 stage cycles (total / min / max):

- ready_to_issue: 1992 / 26 / 42
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 36734 / 48 / 2921
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 2471 / 7 / 1406
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 564 / 0 / 14
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 20 / 0 / 20
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 578 / 4 / 14

Fixed example last_finishing_parent: lane 0, j 63, parent CSV line 65; ready 75695, issue 75725, backend 75727, first local R 76290, all source 76832, last B 76839, DONE 76843.

Fixed example longest_latency_V_tie_lowest_j: lane 0, j 0, parent CSV line 2; ready 0, issue 47, backend 49, first local R 2006, all source 2548, last B 5033, DONE 5037.

### runs/base-pn-r16/attempt01


Translation observations:

- Physical first lookup checks / N probes: {"n_probe_hit": 130, "n_probe_miss": 1, "normal_hit": 315, "normal_miss": 67}.
- Unique accepted logical requests: {"n_accepted": 130, "normal_accepted": 382, "normal_first_check_hit": 315, "normal_first_check_miss": 67, "normal_with_walk": 67}.
- Unique PTW sequences: 67; grouped by actual owner/page: {"lane0_destination_all": 1, "lane0_source_all": 1, "lane1_destination_after_ringlap": 48, "lane1_destination_all": 64, "lane1_destination_repeated_page": 48, "lane1_source_all": 1}.

Behavioral DDR model truth (total / min / max across read occurrences):

- queued_cycles_before_head: 83299 / 0 / 564
- startup_ticks: 14229 / 31 / 31
- backpressure_ticks: 208 / 0 / 104
- valid_ready_ticks: 65753 / 1 / 256
- other_head_ticks: 0 / 0 / 0

Queue distance is separately retained as raw acceptance-to-head edge distance. Startup classification uses the explicit setup-wait flag, including the final decrement-to-zero tick. These are model observations, not controller-visible information.

Supply and overlap remain separate: {"both_lanes_issue_to_done": 125743, "either_lane_issue_to_done": 130036, "interpretation": "descriptive interval intersection, not recoverable or saved time"}.

AW flags can coexist; no unique root-cause precedence is assigned: {"by_lane": {"0": {"each_flag_cycles_not_additive": {"0": 92898, "1": 0, "2": 0, "3": 0}, "stall_cycles": 92898}, "1": {"each_flag_cycles_not_additive": {"0": 29430, "1": 0, "2": 0, "3": 0}, "stall_cycles": 29430}}, "flag_semantics": ["open write selection conflict", "ID/count capacity", "occupied AW ID mapped to another selection", "selected downstream AW valid and not ready"], "stall_cycles_union": 122328}.

Lane 0 stage cycles (total / min / max):

- ready_to_issue: 1691 / 26 / 47
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 11387 / 79 / 1957
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 81928 / 1261 / 2485
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 278 / 4 / 20
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 5 / 0 / 5
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 262 / 4 / 10

Lane 1 stage cycles (total / min / max):

- ready_to_issue: 2174 / 29 / 42
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 6007 / 48 / 2921
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 86313 / 7 / 2296
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 746 / 0 / 12
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 20 / 0 / 20
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 758 / 7 / 12

Fixed example last_finishing_parent: lane 0, j 63, parent CSV line 65; ready 128092, issue 128118, backend 128120, first local R 128271, all source 128813, last B 130074, DONE 130078.

Fixed example longest_latency_V_tie_lowest_j: lane 0, j 0, parent CSV line 2; ready 0, issue 47, backend 49, first local R 2006, all source 2548, last B 5033, DONE 5037.

### runs/base-pn-r8/attempt01


Translation observations:

- Physical first lookup checks / N probes: {"n_probe_hit": 74, "n_probe_miss": 1, "normal_hit": 399, "normal_miss": 39}.
- Unique accepted logical requests: {"n_accepted": 74, "normal_accepted": 438, "normal_first_check_hit": 399, "normal_first_check_miss": 39, "normal_with_walk": 39}.
- Unique PTW sequences: 39; grouped by actual owner/page: {"lane0_destination_all": 1, "lane0_source_all": 1, "lane1_destination_after_ringlap": 28, "lane1_destination_all": 36, "lane1_destination_repeated_page": 28, "lane1_source_all": 1}.

Behavioral DDR model truth (total / min / max across read occurrences):

- queued_cycles_before_head: 83345 / 0 / 564
- startup_ticks: 11625 / 31 / 31
- backpressure_ticks: 208 / 0 / 104
- valid_ready_ticks: 65669 / 1 / 256
- other_head_ticks: 0 / 0 / 0

Queue distance is separately retained as raw acceptance-to-head edge distance. Startup classification uses the explicit setup-wait flag, including the final decrement-to-zero tick. These are model observations, not controller-visible information.

Supply and overlap remain separate: {"both_lanes_issue_to_done": 102022, "either_lane_issue_to_done": 106500, "interpretation": "descriptive interval intersection, not recoverable or saved time"}.

AW flags can coexist; no unique root-cause precedence is assigned: {"by_lane": {"0": {"each_flag_cycles_not_additive": {"0": 69121, "1": 0, "2": 0, "3": 0}, "stall_cycles": 69121}, "1": {"each_flag_cycles_not_additive": {"0": 29486, "1": 0, "2": 0, "3": 0}, "stall_cycles": 29486}}, "flag_semantics": ["open write selection conflict", "ID/count capacity", "occupied AW ID mapped to another selection", "selected downstream AW valid and not ready"], "stall_cycles_union": 98607}.

Lane 0 stage cycles (total / min / max):

- ready_to_issue: 1932 / 26 / 47
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 13704 / 79 / 1957
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 55834 / 329 / 2485
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 519 / 4 / 20
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 5 / 0 / 5
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 505 / 4 / 11

Lane 1 stage cycles (total / min / max):

- ready_to_issue: 2118 / 28 / 42
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 9517 / 48 / 2921
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 59323 / 7 / 2296
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 690 / 0 / 14
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 20 / 0 / 20
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 698 / 6 / 14

Fixed example last_finishing_parent: lane 0, j 63, parent CSV line 65; ready 105518, issue 105546, backend 105548, first local R 105667, all source 106209, last B 106538, DONE 106542.

Fixed example longest_latency_V_tie_lowest_j: lane 0, j 0, parent CSV line 2; ready 0, issue 47, backend 49, first local R 2006, all source 2548, last B 5033, DONE 5037.

### runs/diag-p-r16-2m/attempt01


Translation observations:

- Physical first lookup checks / N probes: {"normal_hit": 509, "normal_miss": 3}.
- Unique accepted logical requests: {"n_accepted": 0, "normal_accepted": 512, "normal_first_check_hit": 509, "normal_first_check_miss": 3, "normal_with_walk": 3}.
- Unique PTW sequences: 3; grouped by actual owner/page: {"lane0_destination_all": 1, "lane0_source_all": 1, "lane1_destination_all": 1}.

Behavioral DDR model truth (total / min / max across read occurrences):

- queued_cycles_before_head: 160704 / 0 / 839
- startup_ticks: 8246 / 31 / 31
- backpressure_ticks: 208 / 0 / 104
- valid_ready_ticks: 65560 / 1 / 256
- other_head_ticks: 0 / 0 / 0

Queue distance is separately retained as raw acceptance-to-head edge distance. Startup classification uses the explicit setup-wait flag, including the final decrement-to-zero tick. These are model observations, not controller-visible information.

Supply and overlap remain separate: {"both_lanes_issue_to_done": 71235, "either_lane_issue_to_done": 75728, "interpretation": "descriptive interval intersection, not recoverable or saved time"}.

AW flags can coexist; no unique root-cause precedence is assigned: {"by_lane": {"0": {"each_flag_cycles_not_additive": {"0": 35998, "1": 0, "2": 0, "3": 0}, "stall_cycles": 35998}, "1": {"each_flag_cycles_not_additive": {"0": 33473, "1": 0, "2": 0, "3": 0}, "stall_cycles": 33473}}, "flag_semantics": ["open write selection conflict", "ID/count capacity", "occupied AW ID mapped to another selection", "selected downstream AW valid and not ready"], "stall_cycles_union": 69471}.

Lane 0 stage cycles (total / min / max):

- ready_to_issue: 2015 / 26 / 47
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 35958 / 65 / 1812
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 2725 / 7 / 1536
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 602 / 4 / 20
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 5 / 0 / 5
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 587 / 4 / 14

Lane 1 stage cycles (total / min / max):

- ready_to_issue: 1988 / 26 / 42
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 36692 / 125 / 2391
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 1444 / 7 / 442
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 560 / 0 / 14
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 20 / 0 / 20
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 569 / 4 / 14

Fixed example last_finishing_parent: lane 0, j 63, parent CSV line 65; ready 74622, issue 74658, backend 74660, first local R 75217, all source 75759, last B 75766, DONE 75770.

Fixed example longest_latency_V_tie_lowest_j: lane 0, j 0, parent CSV line 2; ready 0, issue 47, backend 49, first local R 1861, all source 2403, last B 3939, DONE 3943.

### runs/diag-pn-r16-2m/attempt01


Translation observations:

- Physical first lookup checks / N probes: {"n_probe_hit": 2, "normal_hit": 509, "normal_miss": 3}.
- Unique accepted logical requests: {"n_accepted": 0, "normal_accepted": 512, "normal_first_check_hit": 509, "normal_first_check_miss": 3, "normal_with_walk": 3}.
- Unique PTW sequences: 3; grouped by actual owner/page: {"lane0_destination_all": 1, "lane0_source_all": 1, "lane1_destination_all": 1}.

Behavioral DDR model truth (total / min / max across read occurrences):

- queued_cycles_before_head: 160704 / 0 / 839
- startup_ticks: 8246 / 31 / 31
- backpressure_ticks: 208 / 0 / 104
- valid_ready_ticks: 65560 / 1 / 256
- other_head_ticks: 0 / 0 / 0

Queue distance is separately retained as raw acceptance-to-head edge distance. Startup classification uses the explicit setup-wait flag, including the final decrement-to-zero tick. These are model observations, not controller-visible information.

Supply and overlap remain separate: {"both_lanes_issue_to_done": 71235, "either_lane_issue_to_done": 75728, "interpretation": "descriptive interval intersection, not recoverable or saved time"}.

AW flags can coexist; no unique root-cause precedence is assigned: {"by_lane": {"0": {"each_flag_cycles_not_additive": {"0": 35998, "1": 0, "2": 0, "3": 0}, "stall_cycles": 35998}, "1": {"each_flag_cycles_not_additive": {"0": 33473, "1": 0, "2": 0, "3": 0}, "stall_cycles": 33473}}, "flag_semantics": ["open write selection conflict", "ID/count capacity", "occupied AW ID mapped to another selection", "selected downstream AW valid and not ready"], "stall_cycles_union": 69471}.

Lane 0 stage cycles (total / min / max):

- ready_to_issue: 2015 / 26 / 47
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 35958 / 65 / 1812
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 2725 / 7 / 1536
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 602 / 4 / 20
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 5 / 0 / 5
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 587 / 4 / 14

Lane 1 stage cycles (total / min / max):

- ready_to_issue: 1988 / 26 / 42
- issue_to_backend: 128 / 2 / 2
- backend_to_first_r: 36692 / 125 / 2391
- first_r_to_source_complete: 34688 / 542 / 542
- source_complete_to_last_b: 1444 / 7 / 442
- last_b_to_done: 256 / 4 / 4

Supply and completion-discovery gaps:

- ready_to_cfg: 560 / 0 / 14
- cfg: 1280 / 20 / 20
- cfg_end_to_next: 20 / 0 / 20
- next: 320 / 5 / 5
- hardware_done_to_poll_end: 569 / 4 / 14

Fixed example last_finishing_parent: lane 0, j 63, parent CSV line 65; ready 74622, issue 74658, backend 74660, first local R 75217, all source 75759, last B 75766, DONE 75770.

Fixed example longest_latency_V_tie_lowest_j: lane 0, j 0, parent CSV line 2; ready 0, issue 47, backend 49, first local R 1861, all source 2403, last B 3939, DONE 3943.

### P/PN complete closed-loop differences

Positive PN−P means PN completes later; all values are simulation steps.

- R=1, leaf=4k: {"T_A": 543, "T_V": 543, "T_data": 543, "T_return": 538}; normalized PN/P T_return=1.007048. Total observed closed-loop policy difference, not isolated PTW saving.
- R=8, leaf=4k: {"T_A": 16281, "T_V": 16281, "T_data": 16281, "T_return": 16280}; normalized PN/P T_return=1.180308. Total observed closed-loop policy difference, not isolated PTW saving.
- R=16, leaf=4k: {"T_A": 28773, "T_V": 28773, "T_data": 28773, "T_return": 28770}; normalized PN/P T_return=1.283913. Total observed closed-loop policy difference, not isolated PTW saving.
- R=16, leaf=2m: {"T_A": 0, "T_V": 0, "T_data": 0, "T_return": 0}; normalized PN/P T_return=1.000000. Both 2MiB runs have no accepted N requests; equal complete times do not establish equivalence in other mapping/eligibility conditions.

### Ordinary A-only 2MiB mapping diagnostics

- p: {"T_A": -25597, "T_V": -25535, "T_data": -25535, "T_return": -25537}; normalized 2MiB/4KiB T_return=0.747992. Ordinary mapping-organization intervention on the authorized exclusive A arena. PN N qualification changes with 2MiB leaf; full delta is not pure PTW saving or evidence for general 4KiB-constrained domains.
- pn: {"T_A": -54370, "T_V": -54308, "T_data": -54308, "T_return": -54307}; normalized 2MiB/4KiB T_return=0.582588. Ordinary mapping-organization intervention on the authorized exclusive A arena. PN N qualification changes with 2MiB leaf; full delta is not pure PTW saving or evidence for general 4KiB-constrained domains.

### Bounded causal interpretation

R16 repeated A destination walks are real and necessary: each 4KiB policy has 64 destination walks, including 48 after the first ringlap. The diagnostic gate retains A j16 normal-write acceptance, first lookup miss, own PTW/PTE chain, source completion before normal translation response, and first actual destination AW after that response. The AW-demux stall flags are separate context; the trigger does not require or reinterpret them as translation-bank waiting.

R8 also has repeated destination walks: 36 A destination walks, 28 repeated, in each policy. Footprint below 14 assigned ways does not imply only initial fills; explaining the actual victim/replacement sequence requires critical-source or victim-way evidence beyond footprint arithmetic. No actual victim-way capture is claimed here.

The optional ordinary A-only 2MiB leaf removes repeated A destination walks in this authorized exclusive arena; both policies have three total initial walks and identical complete times. Payload work and all 128 parents remain. This supports an ordinary mapping-organization resolution for this selected arena. It does not establish a mechanism contribution, resolve arbitrary 4KiB-constrained domains, or assign the entire 4KiB→2MiB time difference to pure PTE waiting. PN has zero accepted N requests under this 2MiB eligibility condition.

Supply and completion discovery have visible nonzero gaps, and final B→DONE is four steps per parent. They are explicitly included in complete times. They do not explain away the j16 source-complete→necessary-write-translation chain; no exhaustive single-cause allocation of all cycles is made.

### Committed payload predecessor example

A j16 second PTE: P ID 48/occ 127 accepts at 27750, becomes head at 27751, returns at 27782; PN ID 48/occ 129 accepts at 35006, becomes head at 35468, returns at 35499. PN queued wait before head is 461; own startup remains 31. Exact accepted-ahead payload records are retained in committed-debt-example.json. FIFO behavioral-DDR requests accepted earlier retain service order. DS-first selects only before AR commitment; it cannot preempt already accepted payload. Exact prior request occurrences, head state and R returns establish this concrete predecessor chain. This is not a saved-time estimate or a controller experiment.
- Prior payload lane 0, parent 16, DDR ID 32/occ 127: accepts 34790 (event line 87989), head 34894, first/last R 34925/35180.
- Prior payload lane 0, parent 16, DDR ID 32/occ 128: accepts 34796 (event line 88015), head 35181, first/last R 35212/35467.

Overlap is a descriptive intersection and is never summed into recoverable time. All 128 parents, including cold prefixes and natural final tail, remain in the main result. Failed attempts remain separately visible.

Diagnostic trigger: True. This gate requires exact raw matching evidence and root review; it predicts no optional-diagnostic outcome.

Fallacy scan 11/11: Simpson/ecological inference—no cross-group aggregate inference; Berkson/collider—no post-hoc condition or completion selection for base cases; base-rate/regression-to-mean—no population or repeated-measure claim; survivorship—every attempt retained and failures excluded only from performance comparison; look-elsewhere/forking paths—fixed six bases and preconditioned optional two, all parents retained; correlation/causation/reverse direction—temporal chains descriptive and policy/leaf interventions interpreted only within their actual scope. No p-value, confidence interval or statistical-significance claim is made.

Functional RTL simulation is not native CPU/Linux execution, measured OS allocation, FPGA timing, or a new paper mechanism.
