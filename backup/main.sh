#! /bin/bash

sqlite3 ../Date/hello.db ".dump [mytabl%]" > '%Y%m%d'.sql
# sqlite3 sqb.db < sqa.sql