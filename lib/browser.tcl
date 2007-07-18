# git-gui tree browser
# Copyright (C) 2006, 2007 Shawn Pearce

class browser {

field w
field browser_commit
field browser_path
field browser_files  {}
field browser_status {Starting...}
field browser_stack  {}
field browser_busy   1

field ls_buf     {}; # Buffered record output from ls-tree

constructor new {commit {path {}}} {
	global cursor_ptr M1B
	make_toplevel top w
	wm title $top "[appname] ([reponame]): File Browser"

	set browser_commit $commit
	set browser_path $browser_commit:$path

	label $w.path \
		-textvariable @browser_path \
		-anchor w \
		-justify left \
		-borderwidth 1 \
		-relief sunken \
		-font font_uibold
	pack $w.path -anchor w -side top -fill x

	frame $w.list
	set w_list $w.list.l
	text $w_list -background white -borderwidth 0 \
		-cursor $cursor_ptr \
		-state disabled \
		-wrap none \
		-height 20 \
		-width 70 \
		-xscrollcommand [list $w.list.sbx set] \
		-yscrollcommand [list $w.list.sby set]
	$w_list tag conf in_sel \
		-background [$w_list cget -foreground] \
		-foreground [$w_list cget -background]
	scrollbar $w.list.sbx -orient h -command [list $w_list xview]
	scrollbar $w.list.sby -orient v -command [list $w_list yview]
	pack $w.list.sbx -side bottom -fill x
	pack $w.list.sby -side right -fill y
	pack $w_list -side left -fill both -expand 1
	pack $w.list -side top -fill both -expand 1

	label $w.status \
		-textvariable @browser_status \
		-anchor w \
		-justify left \
		-borderwidth 1 \
		-relief sunken
	pack $w.status -anchor w -side bottom -fill x

	bind $w_list <Button-1>        "[cb _click 0 @%x,%y];break"
	bind $w_list <Double-Button-1> "[cb _click 1 @%x,%y];break"
	bind $w_list <$M1B-Up>         "[cb _parent]        ;break"
	bind $w_list <$M1B-Left>       "[cb _parent]        ;break"
	bind $w_list <Up>              "[cb _move -1]       ;break"
	bind $w_list <Down>            "[cb _move  1]       ;break"
	bind $w_list <$M1B-Right>      "[cb _enter]         ;break"
	bind $w_list <Return>          "[cb _enter]         ;break"
	bind $w_list <Prior>           "[cb _page -1]       ;break"
	bind $w_list <Next>            "[cb _page  1]       ;break"
	bind $w_list <Left>            break
	bind $w_list <Right>           break

	bind $w_list <Visibility> [list focus $w_list]
	set w $w_list
	if {$path ne {}} {
		_ls $this $browser_commit:$path $path
	} else {
		_ls $this $browser_commit $path
	}
	return $this
}

method _move {dir} {
	if {$browser_busy} return
	set lno [lindex [split [$w index in_sel.first] .] 0]
	incr lno $dir
	if {[lindex $browser_files [expr {$lno - 1}]] ne {}} {
		$w tag remove in_sel 0.0 end
		$w tag add in_sel $lno.0 [expr {$lno + 1}].0
		$w see $lno.0
	}
}

method _page {dir} {
	if {$browser_busy} return
	$w yview scroll $dir pages
	set lno [expr {int(
		  [lindex [$w yview] 0]
		* [llength $browser_files]
		+ 1)}]
	if {[lindex $browser_files [expr {$lno - 1}]] ne {}} {
		$w tag remove in_sel 0.0 end
		$w tag add in_sel $lno.0 [expr {$lno + 1}].0
		$w see $lno.0
	}
}

method _parent {} {
	if {$browser_busy} return
	set info [lindex $browser_files 0]
	if {[lindex $info 0] eq {parent}} {
		set parent [lindex $browser_stack end-1]
		set browser_stack [lrange $browser_stack 0 end-2]
		if {$browser_stack eq {}} {
			regsub {:.*$} $browser_path {:} browser_path
		} else {
			regsub {/[^/]+$} $browser_path {} browser_path
		}
		set browser_status "Loading $browser_path..."
		_ls $this [lindex $parent 0] [lindex $parent 1]
	}
}

method _enter {} {
	if {$browser_busy} return
	set lno [lindex [split [$w index in_sel.first] .] 0]
	set info [lindex $browser_files [expr {$lno - 1}]]
	if {$info ne {}} {
		switch -- [lindex $info 0] {
		parent {
			_parent $this
		}
		tree {
			set name [lindex $info 2]
			set escn [escape_path $name]
			set browser_status "Loading $escn..."
			append browser_path $escn
			_ls $this [lindex $info 1] $name
		}
		blob {
			set name [lindex $info 2]
			set p {}
			foreach n $browser_stack {
				append p [lindex $n 1]
			}
			append p $name
			blame::new $browser_commit $p
		}
		}
	}
}

method _click {was_double_click pos} {
	if {$browser_busy} return
	set lno [lindex [split [$w index $pos] .] 0]
	focus $w

	if {[lindex $browser_files [expr {$lno - 1}]] ne {}} {
		$w tag remove in_sel 0.0 end
		$w tag add in_sel $lno.0 [expr {$lno + 1}].0
		if {$was_double_click} {
			_enter $this
		}
	}
}

method _ls {tree_id {name {}}} {
	set ls_buf {}
	set browser_files {}
	set browser_busy 1

	$w conf -state normal
	$w tag remove in_sel 0.0 end
	$w delete 0.0 end
	if {$browser_stack ne {}} {
		$w image create end \
			-align center -padx 5 -pady 1 \
			-name icon0 \
			-image file_uplevel
		$w insert end {[Up To Parent]}
		lappend browser_files parent
	}
	lappend browser_stack [list $tree_id $name]
	$w conf -state disabled

	set fd [git_read ls-tree -z $tree_id]
	fconfigure $fd -blocking 0 -translation binary -encoding binary
	fileevent $fd readable [cb _read $fd]
}

method _read {fd} {
	append ls_buf [read $fd]
	set pck [split $ls_buf "\0"]
	set ls_buf [lindex $pck end]

	set n [llength $browser_files]
	$w conf -state normal
	foreach p [lrange $pck 0 end-1] {
		set tab [string first "\t" $p]
		if {$tab == -1} continue

		set info [split [string range $p 0 [expr {$tab - 1}]] { }]
		set path [string range $p [expr {$tab + 1}] end]
		set type   [lindex $info 1]
		set object [lindex $info 2]

		switch -- $type {
		blob {
			set image file_mod
		}
		tree {
			set image file_dir
			append path /
		}
		default {
			set image file_question
		}
		}

		if {$n > 0} {$w insert end "\n"}
		$w image create end \
			-align center -padx 5 -pady 1 \
			-name icon[incr n] \
			-image $image
		$w insert end [escape_path $path]
		lappend browser_files [list $type $object $path]
	}
	$w conf -state disabled

	if {[eof $fd]} {
		close $fd
		set browser_status Ready.
		set browser_busy 0
		set ls_buf {}
		if {$n > 0} {
			$w tag add in_sel 1.0 2.0
			focus -force $w
		}
	}
} ifdeleted {
	catch {close $fd}
}

}

class browser_open {

field w              ; # widget path
field w_rev          ; # mega-widget to pick the initial revision

constructor dialog {} {
	make_toplevel top w
	wm title $top "[appname] ([reponame]): Browse Branch Files"
	if {$top ne {.}} {
		wm geometry $top "+[winfo rootx .]+[winfo rooty .]"
	}

	label $w.header \
		-text {Browse Branch Files} \
		-font font_uibold
	pack $w.header -side top -fill x

	frame $w.buttons
	button $w.buttons.browse -text Browse \
		-default active \
		-command [cb _open]
	pack $w.buttons.browse -side right
	button $w.buttons.cancel -text {Cancel} \
		-command [list destroy $w]
	pack $w.buttons.cancel -side right -padx 5
	pack $w.buttons -side bottom -fill x -pady 10 -padx 10

	set w_rev [::choose_rev::new $w.rev {Revision}]
	$w_rev bind_listbox <Double-Button-1> [cb _open]
	pack $w.rev -anchor nw -fill both -expand 1 -pady 5 -padx 5

	bind $w <Visibility> [cb _visible]
	bind $w <Key-Escape> [list destroy $w]
	bind $w <Key-Return> [cb _open]\;break
	tkwait window $w
}

method _open {} {
	if {[catch {$w_rev commit_or_die} err]} {
		return
	}
	set name [$w_rev get]
	destroy $w
	browser::new $name
}

method _visible {} {
	grab $w
	$w_rev focus_filter
}

}
