<html lang="zn_CN"> <head> <meta charset='utf-8'> <title>Split string to multiple rows</title> </head> <body>

Split string to multiple rows for Vertica
==========
This is a Vertica User Defined Functions (UDF) for split string to multple rows.

Syntax:
----------

STRSPLIT ( string [using parameters seperator=':separator'] ) over(...)

Parameters:

 * string: input string.
 * separator: separator character for spliting, default value is ','.
 * (return): concat string of input express on window. 

Examples:
----------

<code><pre>
	select word  from strsplitTEST;
	      word      
	----------------
	 SingleWord
	 Break,On,Comma
	 Break:On:Colon
	(3 rows)
</code></pre>
<code><pre>
	select strsplit(word) over () from strsplittest;
	      word      
	----------------
	 SingleWord
	 Break
	 On
	 Comma
	 Break:On:Colon
	(5 rows)
</code></pre>
<code><pre>
	select strsplit(word using parameters separator=':') over () from strsplittest;
	      word      
	----------------
	 SingleWord
	 Break,On,Comma
	 Break
	 On
	 Colon
	(5 rows)
</code></pre>

Install, test and uninstall:
----------
Befoe build and install, g++ should be available(yum -y groupinstall "Development tools" && yum -y groupinstall "Additional Development" can help on this).

 * Build: make
 * Install: make install
 * Test: make run
 * Uninstall make uninstall

</body> </html>



