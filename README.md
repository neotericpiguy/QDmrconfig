# QDmrconfig

### Description
Qt Frontend to https://github.com/sergev/dmrconfig

Basically a glorified configuration file editor. Only really used and tested
with a BTECH DMR 6x2

### Chirp Import
Import an exported csv from chirp into QDmrconfig. Handy for when you want to grab your friend's chirp frequency list and import it into its own zone in your higher capacity DMR radio.

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/ChirpImport.gif" width="650">

### Filtering
Filtering is possible thanks to regex. Filter any column by regex string if
needed. This is handy when you want to find certain scanlists or zones

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/filter.gif" width="650">

### Field Injection
Freely inject or remove values from fields by cell or by batch selection

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/AddRemove.gif" width="650">

### Automatic Column Update
Update columns based on formulaic offsets

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/ColumnUpdate.gif" width="650">

### FCC Callsign Search
Search FCC website for Callsigns

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/FccSearch.gif" width="650">

### Repeaterbook Search
Search Repeaterbook for repeater near a city

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/RepeaterBookSearch.gif" width="650">

### Debug Mode
Show raw text file that will be generated with cell contents

```
$> ./QDmrconfig -d
```

<img src="https://raw.githubusercontent.com/neotericpiguy/i/master/QDmrconfig/debug.gif" width="650">

### Usage
Configuration Block(ConfBlock) is defined as a group of values arranged in rows and
columns that describe a set of information. This could be a block of digital
channels with Color codes and frequencies or a set of analog channels with
Tones and offsets. Columns values are usually defined in the comment block
right above a Configuration Block. Let's look at ConfBlock of Digital Channels

```
Digital Name              Receive  Transmit Power Scan     TOT RO Admit EncType EncKey Color Slot RxGL RadioID TxContact # DigitalOffset Zone    
    1   DSMPLX1-441.00    441.0    +0       High  6        -   -  -     -       -      1     1    -    0       -         # 1             1       
    2   DSMPLX2-446.5     446.5    +0       High  6        -   -  -     -       -      1     1    -    0       -         # +1            1       
    3   DSMPLX3-446.075   446.075  +0       High  6        -   -  -     -       -      1     1    -    0       -         # +1            1       
    4   DSMPLX4-433.45    433.45   +0       High  6        -   -  -     -       -      1     1    -    0       -         # +1            1       
    5   DSMPLX5-145.79    145.79   +0       High  98,99    -   -  -     -       -      1     1    -    0       -         # +1            1,98,99 
```

The columns up to the pound sign are all pretty self explanitory. The tricky
stuff starts at `DigitalOffset`. This is a QDmrconfig mechanism for auto
populating cells. So the syntax is `ColumnNameOffset`, what ever `ColumnName`
for current row value is modified by the corresponding `ColumnNameOffset`
value.  DSMPLX1's `DigitalOffset` value is 1, so the `Digital` column values is
assigned 1. DSMPLX2's `DigitalOffset` value is +1, so the `Digital` column
values is assigned incremented by +1 from the previous row above.

The last column `Zone` is cross ConfBlock identifier. Zone's are defined in
another ConfBlock but with this column you are able declare Channel 1-5 as
being part of Zone 1

Let's look at Zone ConfBlock

```
Zone    Name             Channels                                       # ZoneOffset ChannelsRange-Analog-Zone-Digital-Zone 
    1   SCAN             1-6,9,203,229-233                              # 1                                                 
    2   TUCSON           7-8,10-63,177-180,218                          # +1                                                
    3   LAS_CRUCES       64-93,155-176                                  # +1                                                
```

ZoneOffset was described already as a column modifier, but what is
`ChannelsRange-Analog-Zone-Digital-Zone` ? We are going to have to break it
down. But the important thing is that the values in the Channels column were
AUTOPOPULATED. The user need not look between Analog ConfBlock and Digital
ConfBlock to know what to throw in those cells. Thanks to 
`ChannelsRange-Analog-Zone-Digital-Zone`.

`ChannelsRange` means the Channels Column will be auto populated with a range of values

`Analog-Zone` means that you should look in the Analog ConfBlock's Zone column

`Digital-Zone` means that you should look in the Digital ConfBlock's Zone column

### Installation

#### Unix, Linux
Tested on archlinux with Qt5 and gcc 10.
You will need Qt and libmongoc

```sh
git clone --recursive "https://github.com/neotericpiguy/QDmrconfig.git"
cd QDmrconfig
make -j
```

