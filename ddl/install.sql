/*****************************
 * Vertica Analytic Database
 *
 * strsplit User Defined Functions
 *
 * Copyright Vertica, 2013
 */

-- Step 1: Create LIBRARY 
\set libfile '\''`pwd`'/lib/strsplit.so\'';
CREATE LIBRARY strsplit AS :libfile;

-- Step 2: Create cube/rollup Factory
\set tmpfile '/tmp/strsplitinstall.sql'
\! cat /dev/null > :tmpfile

\t
\o :tmpfile
select 'CREATE TRANSFORM FUNCTION strsplit AS LANGUAGE ''C++'' NAME '''||obj_name||''' LIBRARY strsplit;' from user_library_manifest where lib_name='strsplit' and obj_name like 'StrSplitFactory%';

\o
\t

\i :tmpfile
\! rm -f :tmpfile
