-- Run inside the user-authorized Terminal app; the Python script validates card identity.
tell application "Terminal"
    set sdTab to do script "sudo /opt/homebrew/bin/python3 '/Users/jojo/Desktop/在mac链接pfga(codex)/scripts/sd_card.py' update --disk /dev/disk5"
    activate
    return id of front window
end tell
