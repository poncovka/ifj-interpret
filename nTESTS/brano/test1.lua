-- Program 1: Vypocet faktorialu (iterativne)
function main() --Hlavni telo programu
	local a;
	local vysl;
	
	write("Zadejte cislo pro \" vypocet faktorialu\n" .. "str\n");
	--a = read("*n");
	a = 6;
	
	if a < 0 then
		write("Faktorial nelze spocitat\n");
	else
		vysl = 1;
		while a > 0 do
			vysl = vysl * a;
			a = a - 1;
		end;
		write("Vysledek je: ", vysl, "\n");
	end;
	
end;
