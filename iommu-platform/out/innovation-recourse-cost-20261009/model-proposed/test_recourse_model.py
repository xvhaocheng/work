import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from recourse_model import A_N, B_N, Q, simulate, check_trace, get_scenario


class TestRecourseModel(unittest.TestCase):
    def test_adversarial_late_gap_recourse_patch_after_original_b(self):
        scenario = get_scenario('late_gap_ready_peer')
        sim = simulate('recourse', scenario)
        checker = check_trace(sim['events'], scenario, 'recourse')
        self.assertTrue(checker['ok'], checker['errors'])
        orig_b = [e for e in sim['events'] if e['type'] == 'B_physical' and e.get('kind') == 'original']
        self.assertEqual(len(orig_b), 1)
        self.assertTrue(orig_b[0]['absorbed'])
        self.assertIsNone(orig_b[0]['user_ack'])
        patch_aw = [e for e in sim['events'] if e['type'] == 'AW' and e.get('kind') == 'patch']
        self.assertEqual(len(patch_aw), 1)
        self.assertEqual(patch_aw[0]['length'], 3)
        self.assertEqual(patch_aw[0]['data_start'], 21)
        patch_b = [e for e in sim['events'] if e['type'] == 'B_physical' and e.get('kind') == 'patch']
        self.assertEqual(len(patch_b), 1)
        self.assertEqual(patch_b[0]['user_ack'], 'A')
        a_ack = [e for e in sim['events'] if e['type'] == 'ACK' and e['job'] == 'A']
        self.assertEqual(len(a_ack), 1)
        self.assertEqual(a_ack[0]['cycle'], patch_b[0]['cycle'])
        zero_w = [e for e in sim['events'] if e['type'] == 'W' and e['mode'] == 'null']
        self.assertEqual(len(zero_w), 3)
        true_a = [e for e in sim['events'] if e['type'] == 'W' and e['job'] == 'A' and e['mode'] == 'true']
        self.assertEqual(len(true_a), A_N)
        self.assertEqual(sorted(e['data_index'] for e in true_a), list(range(A_N)))
        self.assertTrue(all(e['data_index'] == e['value'] for e in true_a))

    def test_short_gap_recourse_r_during_null_and_patch_waits_resident(self):
        scenario = get_scenario('late_short_gap')
        sim = simulate('recourse', scenario)
        checker = check_trace(sim['events'], scenario, 'recourse')
        self.assertTrue(checker['ok'], checker['errors'])
        conv = [e for e in sim['events'] if e['type'] == 'CONVERT']
        self.assertEqual(len(conv), 1)
        self.assertEqual(conv[0]['cycle'], 22)
        self.assertEqual(conv[0]['remaining'], Q)
        self.assertEqual(conv[0]['offset'], A_N - Q)
        r21 = [e for e in sim['events'] if e['type'] == 'R' and e['job'] == 'A' and e['data_index'] == 21][0]
        self.assertEqual(r21['cycle'], 24)
        r23 = [e for e in sim['events'] if e['type'] == 'R' and e['job'] == 'A' and e['data_index'] == 23][0]
        self.assertEqual(r23['cycle'], 26)
        patch_aw = [e for e in sim['events'] if e['type'] == 'AW' and e.get('kind') == 'patch'][0]
        self.assertGreaterEqual(patch_aw['cycle'], 27)
        a_ack = [e for e in sim['events'] if e['type'] == 'ACK' and e['job'] == 'A'][0]
        orig_b = [e for e in sim['events'] if e['type'] == 'B_physical' and e.get('kind') == 'original'][0]
        self.assertGreater(a_ack['cycle'], orig_b['cycle'])

    def test_tail_guard_prefix_b_absorbed_tail_final_and_hidden_setup_no_b(self):
        scenario = get_scenario('continuous_late_peer')
        sim = simulate('tail_guard', scenario)
        checker = check_trace(sim['events'], scenario, 'tail_guard')
        self.assertTrue(checker['ok'], checker['errors'])
        prefix_aw = [e for e in sim['events'] if e['type'] == 'AW' and e.get('kind') == 'prefix'][0]
        tail_aw = [e for e in sim['events'] if e['type'] == 'AW' and e.get('kind') == 'tail'][0]
        self.assertEqual(prefix_aw['length'], A_N - Q)
        self.assertEqual(tail_aw['length'], Q)
        prefix_b = [e for e in sim['events'] if e['type'] == 'B_physical' and e.get('kind') == 'prefix'][0]
        self.assertTrue(prefix_b['absorbed'])
        self.assertIsNone(prefix_b['user_ack'])
        tail_b = [e for e in sim['events'] if e['type'] == 'B_physical' and e.get('kind') == 'tail'][0]
        self.assertEqual(tail_b['user_ack'], 'A')
        scenario0 = get_scenario('continuous_no_peer')
        sim0 = simulate('recourse', scenario0)
        self.assertFalse(any(e.get('job') == 'B' for e in sim0['events']))
        self.assertTrue(check_trace(sim0['events'], scenario0, 'recourse')['ok'])


if __name__ == '__main__':
    unittest.main()
