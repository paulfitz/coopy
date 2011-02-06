
Operation of widesheet:
* Independent of version control method.
* But still needs common ancestor (pivot), local and remote.

Files:
  User's file (e.g. /USER/sheet.xls)
    Stored in arbitrary location.  Under user's control, via
    random applications.
  Control directory (e.g. /CONTROL/share1)
    Under widesheet control.  Not intended to be touched by
    end user.
  Config file (/CONTROL/share1/config.sqlite)
    Stores pointer to user's file
    Stores info about version control mechanism
  Version control directory (/CONTROL/share1/repo)
    This is under the control of a vcs, e.g. git, fossil, svn, ...
  Pivot directory (/CONTROL/share1/pivot)

Version control commands:
  VCS_PUSH
    Push out the sheet file(s) to the online repo.
    This should abort if a change has happened in online repo.
  VCS_PULL
    Bring in any changes in sheet file(s) from online repo.

widesheet actions:
  PUSH
    Convert user's file to /USER/share1/repo/repo.csvs.
    Do VCS_PUSH.  On success, copy to pivot at 
    /USER/share1/pivot/repo.csvs.  On failure (change-abort),
    revert /USER/share1/repo/repo.csvs and tell user to PULL.

  PULL
    Do VCS_PULL.  Should never be a merge-fail with widesheet's
    usage method.
    Compute patch from user's file to /USER/share1/repo/repo.csvs,
    and report it to the user or apply it (if possible).
    If applied, move on to APPLIED.

  APPLIED
    Convert user's file to /USER/share1/tmp.csvs and
    compare it with repo/repo.csvs.  If equal, copy rep/repo.csvs
    to pivot/repo.csvs.  Otherwise flag issue.

===

Problem:
* Rely on user to signal "APPLIED" before moving on to make
  changes to their file.  Requires discipline.  Seems unlikely.
  If APPLIED is not called, then common ancestor will normally
  be a generation earlier than needed - only gets updated
  upon a PUSH.
* Not terrible, and anyway will be seeking patch application
  automation.  This is an issue for complex formats that it 
  is best to patch via whatever application the user prefers,
  rather than inplace.  So e.g. using script plugin on Excel/
  Access.
  


