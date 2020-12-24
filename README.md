# QDmrconfig

### Description
Qt Frontend to https://github.com/sergev/dmrconfig

Basically a glorified configuration file editor. Only really used and tested
with a BTECH DMR 6x2

### Filtering
Filtering is possible thanks to regex. Filter any column by regex string if
needed. This is handy when you want to find certain scanlists or zones

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/filter.gif" width="500">

### Field Injection
Freely inject or remove values from fields by cell or by batch selection

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/AddRemove.gif" width="500">

### Automatic Column Update
Update columns based on formulaic offsets

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/ColumnUpdate.gif" width="500">

### FCC Callsign Search
Search FCC website for Callsigns

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/FccSearch.gif" width="500">

### Repeaterbook Search
Search Repeaterbook for repeater near a city

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/RepeaterBookSearch.gif" width="500">

### Debug Mode
Show raw text file that will be generated with cell contents

```
$> ./QDmrconfig -d
```

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/debug.gif" width="500">

### Installation

#### Unix, Linux
Tested on archlinux with Qt5 and gcc 10.
```sh
git clone --recursive "https://github.com/neotericpiguy/QDmrconfig.git"
cd QDmrconfig
make -j check
```
