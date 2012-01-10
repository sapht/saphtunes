to close_split_windows()
	tell application "System Events"
		tell process "Reason"
			repeat
				set reloop to false
				repeat with win in every window
					set wintitle to (title of win)
					log wintitle

					if (wintitle) contains " (Rack)" then
						log "ya gotta do it again (rack)"
						set reloop to true
						set n to number of every window
            click button 1 of win
            --keystroke "w" using {command down}

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
            click button 1 of win
            --keystroke "w" using {command down}

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

to resolve_bad_format()
	log "Bad format"
	tell application "System Events" to key code 52
	tell application "System Events"
		tell process "Reason"
			repeat
				if number of every window is 0 then -- make sure the window is actually closed before we return
					exit repeat
				end if
			end repeat
		end tell
	end tell
end resolve_bad_format

to resolve_missing_sounds()
	log "Missing sounds"
	tell application "System Events" to key code 53
	tell application "System Events"
		tell process "Reason"
			repeat
				if number of every window is 0 then -- make sure the window is actually closed before we return
					exit repeat
				end if
			end repeat
		end tell
	end tell
end resolve_missing_sounds

to resolve_please_insert_disk()
	log "Missing refill"
	(* Perform hacks to press escape multiple times *)
	tell application "System Events"
		tell process "Reason"
			key code 53
			repeat
				set n to number of every window
				if n is greater than 0 then
					my resolve_missing_sounds()
					exit repeat
				end if
			end repeat
		end tell
	end tell
end resolve_please_insert_disk

to process_file(current_file_alias, new_name)
  if (length of current_file_alias is greater than 3) then -- 3 is an arbitrary value. this is for testing against empty strings at the end of the file list... perhaps is should sanitize that variable instead ;)
    tell application "System Events"
      tell application "Reason"
        open current_file_alias -- if this fails, the script should choke entirely, which is good
      end tell
      tell application process "Reason"
        repeat
          my clog("Inside repeat...")
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
              -- BAD FORMAT
              my clog("confused???")
              if number of all_ui is 4 then
                log "well..."
                if name of item 4 in all_ui is "File has bad format" then
                  my resolve_bad_format()
                  log "well well..."
                  my clog("bad format: " & current_file_alias)
                  set did_process to true
                  return "broken"
                  exit repeat
                end if
              else

                -- MISSING SOUNDS
                if current_title is "Missing Sounds" then
                  select current_win
                  my resolve_missing_sounds()
                  my clog("missing sounds: " & current_file_alias)
                  set did_process to true
                  return "missing_sounds"
                  exit repeat
                end if

                -- INSERT DISK
                if current_title is "Please Insert Disk" then
                  select current_win
                  my resolve_please_insert_disk()
                  my clog("missing refill: " & current_file_alias)
                  set did_process to true
                  return "missing_refill"
                  exit repeat
                end if

                -- MAIN APPLICATION WINDOW
                my clog((number of all_ui) as string)
                if (number of all_ui) is in {21, 22, 23} then -- looks like this number changed in reason6
                  my close_split_windows()
                  my saphtune_main(current_file_alias, new_name)

                  tell application "System Events"
                    keystroke "w" using {command down}
                  end tell

                  set did_process to true
                  exit repeat
                end if
              end if
            end if
            delay 1
          end repeat

          if did_process is true then
            repeat
              if (number of every window) is 0 then
                exit repeat
              end if

              delay 1 -- this delay is neat because applescript seems to hog a lot of CPU while probing for windows, so it actually makes the process faster
            end repeat
            exit repeat
          end if
        end repeat
      end tell
    end tell
  end if
end process_list
