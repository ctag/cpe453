#!/bin/bash

# Write SQL tables into a server
# Careful! Will delete anything in it's way.

if [ -z $1 || -z $2 || -z $3 || -z $4]
then
echo "Usage:"
echo "./server_clobber.sh [user] [password] [port] [database]"
exit
fi

echo "Username: $1"
echo "Password: $2"
echo "Port: $3"
echo "Database to write tables into: $4"

for sqlFile in *.sql
do
echo "Loading ${sqlFile} into database $1."
#mysql -h pavelow -u root -P 33153 --password=cstrapwi $1 < sqlFile
mysql -h pavelow.ece.uah.edu -u $1 --password=$2 -P $3 $4 < sqlFile
done

