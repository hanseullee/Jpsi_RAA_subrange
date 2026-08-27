1. Perform entire fit process following: /data/users/pjgwak/work/daily_code_tracker/2026/06/26_run2_Raa_subrange_fit/pb_jpsi/run_pp_jpsi_chain.sh
2. Go step by step chekcing fit results:
  1. Especially pull (>0.05), fit converegnece
  2. Becarefuly interperting chi2/ndf of unbinned bin.
3. Becareful to change the model which was not implemented each fit code. 
  1. You can change the model in the each code (mc_mass can't have a bkg model, and mass model can't have simple exponential.)
  2. You can add 5th gaussian for ctau_pr and subrange_ctau if you need
4. If subrange_ctau's pull is bad due to edge bins due to low stats,
  1. Make test code and find a better bin width, ctau rannge.
  2. Then, apply to real codes.
  3. refer to ctau window of /data/users/pjgwak/work/daily_code_tracker/2026/06/26_run2_Raa_subrange_fit/pp_jpsi/roots