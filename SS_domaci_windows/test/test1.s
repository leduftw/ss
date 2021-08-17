lab: .section text # bad
.extern var1   ,   var2   ,    var3    #   ext
.section abc #dusko
.end #dusko
push a # komentar
l: str [r1], r2
lab1:
lab2:
lab3:
lab4: mov r4, r5
#lab4  : .global   a  ,  b    ,c   ,  d   
  lab  :  
.global a
.global a, b
.global q # comment
.end
.equ a, 10
halt
int r6
xchg r2, r3
#bad r1, r2, r3
push  
pop #    erwqeqw
lab: #lab with comment216565234324&*@#$*&^*&#&$&@#&wsdeiufhei
mov r1, *[r1 + %r3 + $5]  # mov r6 to r1
   # only comment  !NULLPTR EXCPT!
.section .text  # komentari komentari
lab:
.long 5, 6, 7, 8, 9
nova_lab: xchg r1, r2
mov r6, $5
lab:
xchg r1, r2 # kom
.long 4, 5, 6, 7
.end
