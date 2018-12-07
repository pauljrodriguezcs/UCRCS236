Redbase-Spatial Project
========================

# Install the dependencies
`sudo apt-get install flex bison g++ g++-multilib git cmake make`

# Clone the repository
`git clone https://github.com/PayasR/redbase-spatial.git`

# Build the project
`cd redbase-spatial`

`mkdir build`

`cd build`

`cmake ..`

`make -j4`

# Test
`./dbcreate test`

`./redbase test`

# Create and insert data
`CREATE TABLE DATA (id i, location m);`

`INSERT INTO DATA VALUES (1, [1,2,3,4]);`

# Select using INTERSECTS
`SELECT * FROM DATA WHERE location INTERSECTS [2,4,3,5]`

`DELETE * FROM DATA WHERE location INTERSECTS [2,4,3,5]`

# Create indexes
`CREATE INDEX DATA (location)`

