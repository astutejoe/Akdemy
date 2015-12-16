int int_byte = 20, int_int = 100000;
byte byte1 = 120, byte2 = 2;
string str1 = "hello", str2 = "world";
boolean bool1 = TRUE, bool2 = FALSE;

begin
	if int_int == int_int
	begin
		writeln "INT EQUAL: OK";
	end
	else
	begin
		writeln "INT EQUAL: NOT OK";
	end

	if int_int > int_int
	begin
		writeln "INT GREATER: NOT OK";
	end
	else
	begin
		writeln "INT GREATER: OK";
	end

	if int_int >= int_int
	begin
		writeln "INT GREATER EQUAL: OK";
	end
	else
	begin
		writeln "INT GREATER EQUAL: NOT OK";
	end

	if int_int < int_int
	begin
		writeln "INT MINOR: NOT OK";
	end
	else
	begin
		writeln "INT MINOR: OK";
	end

	if int_int <= int_int
	begin
		writeln "INT MINOR EQUAL: OK";
	end
	else
	begin
		writeln "INT MINOR EQUAL: NOT OK";
	end

	if int_byte * int_int == (2000000)
	begin
		writeln "INT MULTIPLICATION: OK";
	end
	else
	begin
		writeln "INT MULTIPLICATION: NOT OK";
	end

	if (int_int / int_byte) == (5000)
	begin
		writeln "INT DIVIDING: OK";
	end
	else
	begin
		writeln "INT DIVIDING: NOT OK";
	end

	if int_byte + int_int == (100020)
	begin
		writeln "INT SUM: OK";
	end
	else
	begin
		writeln "INT SUM: NOT OK";
	end

	if int_int - int_byte == (99980)
	begin
		writeln "INT SUBTRACTION: OK";
	end
	else
	begin
		writeln "INT SUBTRACTION: NOT OK";
	end

	if int_byte - int_int == (-99980)
	begin
		writeln "INT NEGATIVE SUBTRACTION: OK";
	end
	else
	begin
		writeln "INT NEGATIVE SUBTRACTION: NOT OK";
	end

	if byte1 == byte1
	begin
		writeln "BYTE EQUAL: OK";
	end
	else
	begin
		writeln "BYTE EQUAL: NOT OK";
	end

	if byte1 > byte1
	begin
		writeln "BYTE GREATER: NOT OK";
	end
	else
	begin
		writeln "BYTE GREATER: OK";
	end

	if byte1 >= byte1
	begin
		writeln "BYTE GREATER EQUAL: OK";
	end
	else
	begin
		writeln "BYTE GREATER EQUAL: NOT OK";
	end

	if byte1 < byte1
	begin
		writeln "BYTE MINOR: NOT OK";
	end
	else
	begin
		writeln "BYTE MINOR: OK";
	end

	if byte1 <= byte1
	begin
		writeln "BYTE MINOR EQUAL: OK";
	end
	else
	begin
		writeln "BYTE MINOR EQUAL: NOT OK";
	end

	if byte2 * byte1 == (240)
	begin
		writeln "BYTE MULTIPLICATION: OK";
	end
	else
	begin
		writeln "BYTE MULTIPLICATION: NOT OK";
	end

	if (byte1 / byte2) == (60)
	begin
		writeln "BYTE DIVIDING: OK";
	end
	else
	begin
		writeln "BYTE DIVIDING: NOT OK";
	end

	if byte2 + byte1 == (122)
	begin
		writeln "BYTE SUM: OK";
	end
	else
	begin
		writeln "BYTE SUM: NOT OK";
	end

	if byte1 - byte2 == (118)
	begin
		writeln "BYTE SUBTRACTION: OK";
	end
	else
	begin
		writeln "BYTE SUBTRACTION: NOT OK";
	end

	if byte2 + (-byte1) == -118
	begin
		writeln "BYTE NEGATIVE SUBTRACTION: OK";
	end
	else
	begin
		writeln "BYTE NEGATIVE SUBTRACTION: NOT OK";
	end

	if str1 == str1
	begin
		writeln "STRING EQUAL: OK";
	end
	else
	begin
		writeln "STRING EQUAL: NOT OK";
	end

	if str1+str2 == "helloworld"
	begin
		writeln "STRING CONCAT: OK";
	end
	else
	begin
		writeln "STRING CONCAT: NOT OK";
	end

	if bool1 == bool1
	begin
		writeln "BOOLEAN EQUAL: OK";
	end
	else
	begin
		writeln "BOOLEAN EQUAL: NOT OK";
	end

	if bool1 or bool2 == TRUE
	begin
		writeln "BOOLEAN OR: OK";
	end
	else
	begin
		writeln "BOOLEAN OR: NOT OK";
	end

	if bool1 and bool2 == FALSE
	begin
		writeln "BOOLEAN AND: OK";
	end
	else
	begin
		writeln "BOOLEAN AND: NOT OK";
	end

	if not bool1 == FALSE
	begin
		writeln "BOOLEAN NOT: OK";
	end
	else
	begin
		writeln "BOOLEAN NOT: NOT OK";
	end

end