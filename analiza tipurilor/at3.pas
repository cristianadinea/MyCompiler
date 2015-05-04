program at03;
var
 	a,b,c:real;
	 i:char;

 	procedure p2;
 	var x,y,z:integer;
	 begin
		  case x of
		   y,z: x:=x+x;
		   'y','z':x:=x+y;
		  end;
	 end;

begin
	 p;p2;
end.