import copy
import unittest
from validate_capture import validate


def fixture():
    w = [0]*4608
    w[0] = 0x4d49474445425402
    w[1] = 0x3ff
    w[3] = 1
    w[5:9] = [1,90,200,200]
    w[11] = 20
    w[12] = 10 | (3 << 32)
    w[13] = 0x10000 | (48 << 30) | (3 << 44) | (1 << 47)
    w[14:18] = [100,105,106,106]
    w[18] = 1
    w[19] = 1 << 16
    w[20] = 1
    w[21] = 2
    w[23:27] = [4,1,2,(1536 << 32) | 32]
    w[28] = 0x5246495245480001
    w[32] = 3 | (36 << 8) | (3 << 14) | (3 << 22) | (1 << 25) | (2 << 27)
    w[33:39] = [9 | (2 << 32),0x2000,89,102,0,2]
    for i,(tick,rid,last,held,ordinal) in enumerate([(100,36,0,0,9),(101,36,0,0,9),
                                                   (102,36,1,0,9),(106,48,1,1,10)]):
        w[512+2*i] = tick
        w[513+2*i] = rid | (last << 6) | (1 << 9) | (1 << 10) | (1 << 11) | ((tick == 100) << 12) | (held << 13) | (ordinal << 32)
    s = [0]*1024
    s[0] = 0x4d49474f574e0001
    s[1] = 5
    s[3:7] = [2,80,190,100]
    s[7:9] = [3,2]
    s[11] = 3 | (3 << 6) | (2 << 16) | (2 << 23)
    s[13:15] = [10,11]
    s[25] = (544 << 32) | 32
    s[26] = (96 << 48) | (5 << 32) | (64 << 16) | 8
    for i in range(3):
        base = 32+8*i
        s[base:base+8] = [80+i,i+1,7 | (10 << 32) | (3 << 56) | (1 << 63),0x80010000,
                              0x4000,(1 << 43) | (3 << 55) | (1 << 58),5,10 | (1 << 34) | (1 << 35) | (1 << 36)]
    for i in range(2):
        base = 544+5*i
        s[base:base+5] = [70+i,i+1,0x80002000,7,20 | (4 << 32) | (3 << 36) | (3 << 44) | (1 << 47) | (1 << 49) | (1 << 51) | (1 << 52)]
    w[0xe00:] = s
    req = {"device":"/dev/idma0","mode":3,"protected_did":10,"victim_gap_cycles":0,"a_pages":8,
           "aggressor_id":1,"victim_pages":1,"chunk_bytes":4096,"victim_transfers":64,
           "warmup_transfers":256,"run_timeout_ms":15000}
    stats = {"submitted":64,"completed":64,"completed_bytes":262144,"data_errors":0,"quarantined":0,"in_flight_at_end":0}
    original = {"schema":"idma-composition-v1","success":True,"run_attempted":True,"request":req,
                "result":{"status":0,"restore_status":0,"raw_captured":1,"raw":[0]*128,
                          "p4m":{"observation":[0]*35,"window":[0]*16},
                          "experiment":{"measurement_started":1,"measurement_complete":1,"status":0,
                                        "victim":stats,"aggressor":copy.deepcopy(stats)}}}
    identity = {k:True for k in ("timing_accepted","bit_identity_verified","reset_to_first_AR_epoch_verified",
                               "exact_source_build_verified","source_ID_mapping_verified")}
    identity["loaded_bit_sha256"] = "0"*64
    return w, original, identity


class ValidatorTest(unittest.TestCase):
    def test_valid_carry_in_same_edge_and_repeated_tuple(self):
        r = validate(*fixture())
        self.assertTrue(r["valid"],r["errors"])
        self.assertEqual(r["payload_debt"],2)
        self.assertEqual(r["decision"],"reachable_payload_R_debt")
        self.assertEqual(r["P_owner"]["occurrence"],3)

    def test_rejects_critical_failures(self):
        for name, change in [
            ("native_error",lambda w,o,i:w.__setitem__(2,1 << 3)),
            ("source_error",lambda w,o,i:w.__setitem__(0xe01,5 | (1 << 32))),
            ("wrong_tuple",lambda w,o,i:w.__setitem__(0xe00+544+5+2,0x80003000)),
            ("non_normal_P",lambda w,o,i:w.__setitem__(0xe00+32+16+2,w[0xe00+32+16+2] & ~(1 << 63))),
            ("unknown_epoch",lambda w,o,i:i.__setitem__("reset_to_first_AR_epoch_verified",False)),
            ("lost_R_fire",lambda w,o,i:w.__setitem__(513+2, w[513+2] & ~(1 << 10))),
            ("bad_held_span",lambda w,o,i:w.__setitem__(519,w[519] | (10 << 13))),
            ("row_RID_spoof",lambda w,o,i:w.__setitem__(32,w[32] & ~(63 << 8))),
            ("wrong_workload",lambda w,o,i:o["request"].__setitem__("a_pages",16)),
            ("wrong_complete_bytes",lambda w,o,i:o["result"]["experiment"]["victim"].__setitem__("completed_bytes",262136)),
            ("ambiguous_occurrence",lambda w,o,i:w.__setitem__(0xe00+32+1,3)),
        ]:
            with self.subTest(name=name):
                w,o,i=fixture();change(w,o,i)
                self.assertFalse(validate(w,o,i)["valid"])

    def test_noeligible_is_separate(self):
        w,o,i=fixture();w[1]=0x4f;w[11]=0
        r=validate(w,o,i)
        self.assertTrue(r["valid"],r["errors"])
        self.assertEqual(r["decision"],"noeligiblePTE")

    def test_original_A_window_end_inflight_is_not_final_quiet(self):
        w,o,i=fixture()
        o["result"]["experiment"]["aggressor"]["in_flight_at_end"] = 1
        r=validate(w,o,i)
        self.assertTrue(r["valid"],r["errors"])
        self.assertEqual(r["original_window_end_in_flight"]["aggressor"],1)

    def test_unknown_DS_ID54_is_not_invented_PQ(self):
        w,o,i=fixture()
        w[32] = (w[32] & ~(63 << 8)) | (54 << 8)
        self.assertFalse(validate(w,o,i)["valid"])


if __name__ == "__main__":
    unittest.main()
