-- User explicitly opted out of payload hash checking and write readback for this release.
on run argv
    if (count of argv) is not 1 then error "Pass verified disk path."
    set diskPath to item 1 of argv
    if diskPath is not "/dev/disk5" then error "Re-check changed device numbering."
    tell application "Terminal"
        do script "sudo /opt/homebrew/bin/python3 '/Users/jojo/Desktop/在mac链接pfga(codex)/scripts/sd_card.py' update --disk " & quoted form of diskPath & " --release dma-cache-sync-20260916 --skip-payload-hash --skip-readback"
        activate
        return id of front window
    end tell
end run
