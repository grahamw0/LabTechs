#! /bin/csh -f

# Author: Will Graham
# Date: 8 December 2016

#set echo

@ total = 0  # Could be by piping ls to wc.  Oh well.
@ unchanged = 0
@ preExist = 0
# Changed will be total - (unchanged + preExist)

if ($#argv == 0) then
    set thedir = .
else
    set thedir = $argv[1]
endif
    
if ( ! -d $thedir ) then
    echo "$thedir is not a directory"
    exit
endif

if ( ! -r $thedir ) then
    echo "$thedir is not readable"
    exit
endif

set numfiles = `ls -a $thedir | wc -l`
if ( $numfiles == 2) then
    echo "$thedir is empty"
    exit
endif

cd $thedir

foreach file (*)
    @ total++
    set newname = `echo $file | tr A-Z a-z`
    @ fileCount = `ls | grep -ixc $file`
    if( $file == $newname ) then
	@ unchanged++
	echo "$file remains unchanged."
    else if ( $fileCount >= 2 ) then
	@ preExist++
	echo "$file's new name would already exist!"
    else
	#mv $file $newname
	echo "Moving $file to $newname"
    endif
end

echo ""
echo "Directory $thedir":
echo "Total files: $total"
echo "Unchanged:   $unchanged"
echo "Preexisting: $preExist"
@ changed = $total - ($unchanged + $preExist)
echo "Changed:     $changed"
