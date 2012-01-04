(* vim: set ft=applescript ts=2 sw=2 sts=2: *)
(* keystroke "e" using {command down, shift down, control down} *)

to saphtune_main(file_alias, new_name)
	tell application "System Events"
		tell process "Reason"
			tell application "Reason" to activate
			
			my menu_click({"Reason", "File", "Save as" & character id 8230})
			
			repeat
				set s to false
				repeat with current_window in every window
					if (get title of current_window) is equal to "Save" then
						set s to true
						exit repeat
					end if
				end repeat
				if (s is true) then
					exit repeat
				end if
			end repeat
			
			--click pop up button 1 of item 5 of every UI element of window 1
			--click menu item "WAVE File" of menu 1 of pop up button 1 of item 5 of every UI element of window 1
			
			keystroke "g" using {command down, shift down}
			set dirname to my get_dirname(file_alias)
			keystroke dirname & "/" -- directory to save. make sure the directory exists, or else the last item will be considered a file name!
      --delay 1
      my clog("go to...")
			key code 52 -- close "go to" dialog

      keystroke new_name
      delay 1
      --delay 1
      my clog("save...")
      --delay 1
      --key code 52 -- press "save"
      click button 4 of window 1
      delay 1

      --delay 1
      my clog("OK")
      --delay 1

      

      repeat
        if (number of every window) is 1 then
          exit repeat
        end if
				
				-- TODO: sometimes, export fails. it says something like "Corrupted file when exporting XYZ."
        delay 1 -- this delay is neat because applescript seems to hog a lot of CPU while probing for windows, so it actually makes the process faster
      end repeat
			
			
			my clog("Updated " & file_alias)
			--click button 1 of window 1 -- should be the "close" button
			return "success"
		end tell
	end tell
end export_song

on run argv
  my process_file( item 1 of argv, item 2 of argv )
end run
