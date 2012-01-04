tell application "Record"
	activate
end tell

(* Extract notes to lanes 
9		--> button "Duplicate" of UI element 5 of window "Tool Window" of application process "Record"
10		--> button "Move" of UI element 5 of window "Tool Window" of application process "Record"
11		--> incrementor 2 of UI element 5 of window "Tool Window" of application process "Record"
12		--> incrementor 3 of UI element 5 of window "Tool Window" of application process "Record"
13		--> incrementor 4 of UI element 5 of window "Tool Window" of application process "Record"
14		--> radio button "Explode" of UI element 5 of window "Tool Window" of application process "Record"
15		--> radio button "Note Range:" of UI element 5 of window "Tool Window" of application process "Record"
16		--> radio button "Single Note:" of UI element 5 of window "Tool Window" of application process "Record"
17		--> pop up button 1 of UI element 5 of window "Tool Window" of application process "Record"
18		--> button "Apply" of UI element 5 of window "Tool Window" of application process "Record"
19		--> incrementor 5 of UI element 5 of window "Tool Window" of application process "Record"
20		--> text field 1 of UI element 5 of window "Tool Window" of application process "Record"
21		--> button "Apply" of UI element 5 of window "Tool Window" of application process "Record"
22		--> button "Half" of UI element 5 of window "Tool Window" of application process "Record"
23		--> button "Double" of UI element 5 of window "Tool Window" of application process "Record"
24		--> incrementor 6 of UI element 5 of window "Tool Window" of application process "Record"
25		--> text field 2 of UI element 5 of window "Tool Window" of application process "Record"
26		--> button "Apply" of UI element 5 of window "Tool Window" of application process "Record"
27		--> button "Apply" of UI element 5 of window "Tool Window" of application process "Record"
28		--> incrementor 7 of UI element 5 of window "Tool Window" of application process "Record"
29		--> incrementor 8 of UI element 5 of window "Tool Window" of application process "Record"
30		--> radio button "Gap by:" of UI element 5 of window "Tool Window" of application process "Record"
31		--> radio button "Overlap:" of UI element 5 of window "Tool Window" of application process "Record"
32		--> radio button "Side By Side (Abut)" of UI element 5 of window "Tool Window" of application process "Record"
33		--> button "Apply" of UI element 5 of window "Tool Window" of application process "Record"
34		--> incrementor 9 of UI element 5 of window "Tool Window" of application process "Record"
35		--> incrementor 10 of UI element 5 of window "Tool Window" of application process "Record"
36		--> incrementor 11 of UI element 5 of window "Tool Window" of application process "Record"
37		--> radio button "Fixed:" of UI element 5 of window "Tool Window" of application process "Record"
38		--> radio button "Sub:" of UI element 5 of window "Tool Window" of application process "Record"
39		--> radio button "Add:" of UI element 5 of window "Tool Window" of application process "Record"
40		--> button "Apply" of UI element 5 of window "Tool Window" of application process "Record"
41		--> incrementor 12 of UI element 5 of window "Tool Window" of application process "Record"
42		--> text field 3 of UI element 5 of window "Tool Window" of application process "Record"
43		--> incrementor 13 of UI element 5 of window "Tool Window" of application process "Record"
44		--> text field 4 of UI element 5 of window "Tool Window" of application process "Record"
45		--> incrementor 14 of UI element 5 of window "Tool Window" of application process "Record"
46		--> text field 5 of UI element 5 of window "Tool Window" of application process "Record"
47		--> incrementor 15 of UI element 5 of window "Tool Window" of application process "Record"
48		--> text field 6 of UI element 5 of window "Tool Window" of application process "Record"
49		--> radio button "Random:" of UI element 5 of window "Tool Window" of application process "Record"
50		--> radio button "Scale:" of UI element 5 of window "Tool Window" of application process "Record"
51		--> radio button "Fixed:" of UI element 5 of window "Tool Window" of application process "Record"
52		--> radio button "Add:" of UI element 5 of window "Tool Window" of application process "Record"
53		--> incrementor 16 of UI element 5 of window "Tool Window" of application process "Record"
54		--> incrementor 17 of UI element 5 of window "Tool Window" of application process "Record"
55		--> incrementor 18 of UI element 5 of window "Tool Window" of application process "Record"
56		--> radio button "Randomize" of UI element 5 of window "Tool Window" of application process "Record"
57		--> radio button "Semi-tones:" of UI element 5 of window "Tool Window" of application process "Record"
58		--> button "Apply" of UI element 5 of window "Tool Window" of application process "Record"
59		--> incrementor 19 of UI element 5 of window "Tool Window" of application process "Record"
60		--> text field 7 of UI element 5 of window "Tool Window" of application process "Record"
61		--> pop up button 2 of UI element 5 of window "Tool Window" of application process "Record"
62		--> pop up button 3 of UI element 5 of window "Tool Window" of application process "Record"
63		--> button "Apply" of UI element 5 of window "Tool Window" of application process "Record"
64		--> button "Create Audio Track" of UI element 5 of window "Tool Window" of application process "Record"
65		--> button "Create InstrumentÉ" of UI element 5 of window "Tool Window" of application process "Record"
66		--> button "Create" of UI element 5 of window "Tool Window" of application process "Record"
67		--> scroll bar 1 of UI element 5 of window "Tool Window" of application process "Record"
68		--> checkbox "Other" of UI element 5 of window "Tool Window" of application process "Record"
*)


tell application "System Events"
	tell application process "Record"
		try
			set w to window "Tool Window"
			set f to UI element 5 of w
			log f
			set inc to incrementor 18 of f
			set b to position of inc
		end try
	end tell
end tell
log b

tell application "Extra Suites"
	log b
	move mouse({1879, 233})
end tell

tell application "AppleScript Editor"
	activate
end tell