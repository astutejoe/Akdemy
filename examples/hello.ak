string str
begin
	str = "Hello!"
	writeln str
	writeln "Write your name: "
	readln str
	write "Your name is ", str, " :3 and your name is "

	if str == "Gabriel"
	begin
		writeln "just the same as mine *----*"
	end
	else
	begin
		writeln "not the same as mine, my name is Gabriel by the way! :D"
	end
end