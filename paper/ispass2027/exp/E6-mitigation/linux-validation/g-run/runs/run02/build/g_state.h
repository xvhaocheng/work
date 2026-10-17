/* Host-only model state. Synthetic observation words are not RTL evidence. */
static unsigned g_scenario, g_event, g_hpm_reads, g_hpm_after_event;
static unsigned g_A_done_event, g_V_launch_event, g_abort_event, g_stop_event;
static unsigned g_abort_writes, g_arms, g_clears, g_software_issues[2];
static unsigned g_word_reads[32], g_hist_reads, g_raw_log_count;
static bool g_hpm_A_pending, g_started[2], g_armed, g_has_a, g_aborted;
static bool g_global_valid, g_path_invalid, g_hist_invalid, g_gap_bad, g_target_bad;
static u64 g_finish_at[2], g_latched_gap, g_hw_error, g_status_extra;
static u64 g_raw_logged[32], g_corrupt_value;
static int g_nofault_lane = -1, g_early_lane = -1, g_inventory_lane = -1;
static int g_corrupt_word = -1, g_bad_hist_bin = -1;
static bool g_started_measurement;
