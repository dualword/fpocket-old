proc highlighting { colorId representation id selection } {
   set id [[atomselect $id $selection] molid]
   puts "highlighting $id"
   mol delrep 0 $id
   mol representation $representation
   mol color ResID
   mol selection $selection
   mol addrep $id
}

set repr "Points 10"
highlighting ResID "Points 10" 0 "resname STP"
set id [[atomselect 0 "protein"] molid]
puts "highlighting $id"
mol representation "Lines"
mol material "Transparent"
mol color Element
mol selection "protein"
mol addrep $id
set id [[atomselect 0 "not protein and not resname STP"] molid]
puts "highlighting $id"
mol representation "Bonds"
mol color Element
mol selection "not protein and not resname STP"
mol addrep $id
