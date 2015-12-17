string x;
begin
	x = "Hello!";
	writeln x;
	writeln "Write your name: ";
	readln x;
	write "Your name is ", x, " :3 and your name is ";

	if x == "Gabriel"
	begin
		writeln "just the same as mine *----*";
	end
	else
	begin
		writeln "not the same as mine, my name is Gabriel by the way! :D";
	end
end