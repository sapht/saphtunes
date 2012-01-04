to clog(msg)
	set theFileReference to open for access "/tmp/saphtune.log" with write permission
	write msg & "
" to theFileReference starting at eof
	close access theFileReference
end clog

to split(someText, delimiter)
	set AppleScript's text item delimiters to delimiter
	set someText to someText's text items
	set AppleScript's text item delimiters to {""} -- restore delimiters to default value
	return someText
end split

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
