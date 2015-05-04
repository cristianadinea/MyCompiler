program p1;

const x=23+7;
      y=x+12;	

var a,b,c:integer;
    d,e:real;
    f,g:array[3..24+x] of char;	
    h,i,j:record
    	a,b:integer;
	c:real;
    end;
    k,l:char;

function p2(a:char; var b[3]:integer) :char;
var m,n:integer;
    p:real;
begin
	write("blocul p2");
end;

begin
	write("blocul p1");
end.     

