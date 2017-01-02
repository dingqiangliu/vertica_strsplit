/*****************************
 * Vertica Analytic Database
 *
 * strsplit User Defined Functions
 *
 * Copyright Vertica, 2013
 */


create table if not exists strsplitTEST(
  word varchar(100)
);

COPY strsplitTEST FROM STDIN DELIMITER '|';
SingleWord
Break,On,Comma
Break:On:Colon
\.

COMMIT;

SELECT word  FROM strsplitTEST;

SELECT strsplit(word) OVER () FROM strsplitTEST;

SELECT strsplit(word using parameters separator=':') OVER () FROM strsplitTEST;

drop table if exists strsplitTEST cascade;
