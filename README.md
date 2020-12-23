# QDmrconfig

### Description
Qt Frontend to https://github.com/sergev/dmrconfig

Basically a glorified configuration file editor. Only really used and tested
with a BTECH DMR 6x2

### Filtering
Filtering is possible thanks to regex. Filter any column by regex string if
needed. This is handy when you want to find certain scanlists or zones

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/filter.gif" height="450">

### Field Injection
Freely inject or remove values from fields by cell or by batch selection

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/AddRemove.gif" height="450">

### Automatic Column Update
Update columns based on formulaic offsets

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/ColumnUpdate.gif" height="450">

### FCC Callsign Search
Search FCC website for Callsigns

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/FccSearch.gif" height="450">

### Debug Mode
Show raw text file that will be generated with cell contents

```
$> ./QDmrconfig -d
```

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/debug.gif" height="450">

### Installation
#### Unix, Linux
Tested on archlinux with Qt5 and gcc 10.
```sh
git clone --recursive "https://github.com/neotericpiguy/QDmrconfig.git"
cd QDmrconfig
make -j check
```
