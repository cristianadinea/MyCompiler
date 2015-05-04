program at02;
const 
 	i=7;
var
 	a,b,c:real;
	 ch:char;
	 function f(var a,b,c:real):integer;

		  function f1(i:integer):real;
		  var
			   i:real;
		  begin
			   a:=a+i;
			   i:=i2;
			   f1:=ch;
		  end;
  
		  function f2:char;
		  begin
			   f2:='X';
			   f:=7;
		  end;

	 begin
		  a(a+f2,b+f,c+i);
		  f(1,2,3,4);
		  f1(i/0.8);
	 end;

	 procedure p;
	 var x,y,z:integer;
	
		  procedure p1;
		  var x,y,z:integer;
		  begin
			   p;
			   p1;
			   x:=y+z+p1;
		  end;

	 begin
		  p1;
		  y:=x+z;
	 end;

begin
	 p;p1;f;f1;
end.