(* vim: set ft=applescript: *)
(* keystroke "e" using {command down, shift down, control down} *)
set stdin to do shell script "cat"
log "hello " & stdin

set interactive to false
set only_new to false
set file_list to {stdin}

log item 1 of file_list

process_list(file_list, 0, 0, interactive) -- list, offset, limit, interactive

to split(someText, delimiter)
	set AppleScript's text item delimiters to delimiter
	set someText to someText's text items
	set AppleScript's text item delimiters to {""} -- restore delimiters to default value
	return someText
end split

to close_split_windows()
	tell application "System Events"
		tell process "Record"
			repeat
				set reloop to false
				repeat with win in every window
					set wintitle to (title of win)
					log wintitle
					
					if (wintitle) contains " (Rack)" then
						log "ya gotta do it again (rack)"
						set reloop to true
						set n to number of every window
						click button 3 of win
						
						repeat -- repeat in order to make sure the window is actually closed
							set n2 to number of every window
							if (n2 is less than n) then
								exit repeat
							end if
						end repeat
					end if
					
					if (wintitle) contains " (Mixer)" then
						log "ya gotta do it again (mixer)"
						
						set reloop to true
						set n to number of every window
						click button 3 of win
						
						repeat -- repeat in order to make sure the window is actually closed
							set n2 to number of every window
							if (n2 is less than n) then
								exit repeat
							end if
						end repeat
					end if
					
					if (reloop is true) then
						exit repeat
					end if
				end repeat
				if reloop is false then
					exit repeat
				end if
			end repeat
		end tell
	end tell
end close_split_windows

to get_dirname(file_alias) -- NOTE: will mess up if you give it a path with trailing /
	set ret_str to ""
	set fpath_chunks to my split(file_alias, "/")
	set n to number of fpath_chunks
	set i to 0
	log fpath_chunks
	repeat
		if (i is greater than 0) then
			set p to item (i + 1) of fpath_chunks
			log p
			if i is equal to (n - 1) then
				exit repeat
			end if
			set ret_str to (ret_str & "/" & p)
		end if
		set i to i + 1
	end repeat
	return ret_str
end get_dirname

to clog(msg)
	
	set theFileReference to open for access "/Volumes/Audio/Workspace/autoexport.log" with write permission
	write msg & "
" to theFileReference starting at eof
	close access theFileReference
end clog

on menu_click(mList)
	local appName, topMenu, r
	
	-- Validate our input
	if mList's length < 3 then error "Menu list is not long enough"
	
	-- Set these variables for clarity and brevity later on
	set {appName, topMenu} to (items 1 through 2 of mList)
	set r to (items 3 through (mList's length) of mList)
	
	-- This overly-long line calls the menu_recurse function with
	-- two arguments: r, and a reference to the top-level menu
	tell application "System Events" to my menu_click_recurse(r, ((process appName)'s ¬
		(menu bar 1)'s (menu bar item topMenu)'s (menu topMenu)))
end menu_click

on menu_click_recurse(mList, parentObject)
	local f, r
	
	-- `f` = first item, `r` = rest of items
	set f to item 1 of mList
	if mList's length > 1 then set r to (items 2 through (mList's length) of mList)
	
	-- either actually click the menu item, or recurse again
	tell application "System Events"
		if mList's length is 1 then
			click parentObject's menu item f
		else
			my menu_click_recurse(r, (parentObject's (menu item f)'s (menu f)))
		end if
	end tell
end menu_click_recurse

to export_song(file_alias, interactive)
	tell application "System Events"
		tell process "Record"
			tell application "Record" to activate
			
			my menu_click({"Record", "File", "Export Song as Audio File…"})
			
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
			
			repeat
				if (number of every window) is 1 then
					exit repeat
				end if
				
				-- TODO: sometimes, export fails. it says something like "Corrupted file when exporting XYZ."
				delay 1 -- this delay is neat because applescript seems to hog a lot of CPU while probing for windows, so it actually makes the process faster
			end repeat
			
			click button 3 of window 1 -- should be the "close" button
			repeat
				set break to false
				repeat with current_window in every window
					if (title of current_window is equal to "") then
						if interactive is false then
							click UI element 5 of window 1 -- it will, AFAIK, always consider exporting the file a "change" and ask to save the file.
						else
							display dialog "go ahead and save the file"
						end if
						set break to true
						exit repeat
					end if
				end repeat
				if (break is true) then
					exit repeat
				end if
			end repeat
			
			-- TODO: i could latch onto this and make .record files out of .rns simply by looking for a file dialog after clicking "save" when it asks to save changes
			
			my clog("Successfully exported: " & file_alias)
			return "success"
		end tell
	end tell
end export_song

to close_bad_format()
	log "Bad format"
	tell application "System Events" to key code 52
	tell application "System Events"
		tell process "Record"
			repeat
				if number of every window is 0 then -- make sure the window is actually closed before we return
					exit repeat
				end if
			end repeat
		end tell
	end tell
	
end close_bad_format

to close_missing_sounds()
	log "Missing sounds"
	tell application "System Events" to key code 53
	tell application "System Events"
		tell process "Record"
			repeat
				if number of every window is 0 then -- make sure the window is actually closed before we return
					exit repeat
				end if
			end repeat
		end tell
	end tell
end close_missing_sounds

to close_please_insert_disk()
	log "Missing refill"
	(* Perform hacks to press escape multiple times *)
	tell application "System Events"
		tell process "Record"
			key code 53
			repeat
				set n to number of every window
				if n is greater than 0 then
					my close_missing_sounds()
					exit repeat
				end if
			end repeat
		end tell
	end tell
end close_please_insert_disk

on rstripString(theText, trimString)
   set x to count trimString
   try
      repeat while theText ends with the trimString
         set theText to characters 1 thru -(x + 1) of theText as text
      end repeat
   on error
      return ""
   end try
   return theText
end rstripString

to process_list(file_list, loop_start, limit, interactive)
	set n to 1
	set loop_iteration to 1
	repeat with current_file_alias in file_list
		log current_file_alias
		set current_wavfile to (rstripstring(rstripstring(current_file_alias, ".record"), ".rns"))
		if ( true or not only_new or (exists file (POSIX file current_wavfile))) then
			if (length of current_file_alias is greater than 3) then -- 3 is an arbitrary value. this is for testing against empty strings at the end of the file list... perhaps is should sanitize that variable instead ;)
				if (loop_iteration is greater than loop_start) then
					if (limit is greater than 0 and n is greater than limit) then
						exit repeat
					end if
					
					tell application "System Events"
						tell application process "Record"
							open current_file_alias -- if this fails, the script should choke entirely, which is good
							
							repeat
								set did_process to false
								repeat with current_win in every window -- the reason for this loop is to circumvent "Open Song" (and possibly other, unknown) tickers
									set loop_continue to false
									try
										set all_ui to every UI element of current_win
										set current_title to (get title of current_win) -- if for some reason the window has disappeared now, we will have an error. that's why we use a try block
									on error
										set loop_continue to true
									end try
									
									if loop_continue is false then -- if the try block failed we will not go here. the try block should only fail during tiny frames of 0 window count
										if number of all_ui is 4 then
											log "well..."
											if name of item 4 in all_ui is "File has bad format" then
												my close_bad_format()
												log "well well..."
												my clog("bad format: " & current_file_alias)
												set did_process to true
												return "broken"
												exit repeat
											end if
										else
											
											if (interactive is false) then
												if current_title is "Missing Sounds" then
													select current_win
													my close_missing_sounds()
													my clog("missing sounds: " & current_file_alias)
													set did_process to true
													return "missing_sounds"
													exit repeat
												end if
												
												if current_title is "Please Insert Disk" then
													select current_win
													my close_please_insert_disk()
													my clog("missing refill: " & current_file_alias)
													set did_process to true
													return "missing_refill"
													exit repeat
												end if
												
												if number of all_ui is 6 then
													-- what happens with tracks that have split windows?		
													my close_split_windows()
													
													return my export_song(current_file_alias, interactive)
													-- don't log this, errors might occur. depend on export_song to do the logging
													set did_process to true
													exit repeat
												end if
											else -- interactive mode
												set perform_export to (button returned of (display dialog "continue?" buttons {"y", "n"}))
												if (perform_export is equal to "n") then
													set did_process to true
													exit repeat
												else
													tell application "Record" to activate
													-- does the loop break here?
													select current_win
													my export_song(current_file_alias, interactive)
													-- don't log this, errors might occur. depend on export_song to do the logging
													set did_process to true
													exit repeat
												end if
											end if
										end if
									end if
									delay 1
								end repeat
								
								if did_process is true then
									exit repeat
								end if
							end repeat
						end tell
					end tell
					set n to n + 1
				end if
				set loop_iteration to loop_iteration + 1
			end if
		end if
	end repeat
end process_list
