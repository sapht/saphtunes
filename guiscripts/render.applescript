(* vim: set ft=applescript: *)
(* keystroke "e" using {command down, shift down, control down} *)
to saphtune_main(file_alias, new_name)
	tell application "System Events"
		tell process "Reason"
			tell application "Reason" to activate
			
			my menu_click({"Reason", "File", "Export Song as Audio File" & character id 8230})

			repeat
				set s to false
				repeat with current_window in every window
					if (get title of current_window) is equal to "Save" then
						set s to true
						exit repeat
					end if
                    delay 1
				end repeat
				if (s is true) then
					exit repeat
				end if
			end repeat
			
			click pop up button 1 of item 5 of every UI element of window 1
			click menu item "WAVE File" of menu 1 of pop up button 1 of item 5 of every UI element of window 1
			
			keystroke "g" using {command down, shift down}
			set dirname to my get_dirname(file_alias)
			keystroke dirname & "/" -- directory to save. make sure the directory exists, or else the last item will be considered a file name!
			key code 52 -- close "go to" dialog
			key code 52 -- press "save"
			
			repeat with current_window in every window
				try
					if (title of current_window is equal to "Audio Export Settings") then
						-- this means we successfully selected a file
						exit repeat
					end if

					set z to every sheet of window 1
					if number of z is 1 then -- in case the file exists, we want to overwrite it
						click UI element 4 of item 1 of z -- this should be the "replace" button
						-- don't rely on tab-navigation using the keyboard, it depends on an OS X syspref
					end if
				end try
			end repeat

			key code 52 -- this closes the dialog where you choose bit depth and sample rate.
            delay 10
			repeat
				if (number of every window) is 1 then
					exit repeat
				end if
				
				delay 1 -- this delay is neat because applescript seems to hog a lot of CPU while probing for windows, so it actually makes the process faster
			end repeat

			
			my clog("exported:" & file_alias)
			return "success"
		end tell
	end tell
end saphtune_main

on run argv
  my process_file( item 1 of argv, item 2 of argv )
end run
