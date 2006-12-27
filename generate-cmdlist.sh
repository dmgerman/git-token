#!/bin/sh

echo "/* Automatically generated by $0 */
struct cmdname_help
{
    char name[16];
    char help[80];
};

struct cmdname_help common_cmds[] = {"

sort <<\EOF |
add
apply
archive
bisect
branch
checkout
cherry-pick
clone
commit
diff
fetch
grep
init-db
log
merge
mv
prune
pull
push
rebase
reset
revert
rm
show
show-branch
status
tag
verify-tag
EOF
while read cmd
do
     sed -n '
     /NAME/,/git-'"$cmd"'/H
     ${
            x
            s/.*git-'"$cmd"' - \(.*\)/  {"'"$cmd"'", "\1"},/
	    p
     }' "Documentation/git-$cmd.txt"
done
echo "};"
