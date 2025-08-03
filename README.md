Have you ever started something, turned around and questioned WTF you're doing?
Enter...

## Dungeon Lords Save Editor (dlse) 

What started as a simple game-save editor, without even any command-line 
options for my own personal use, became a sort of hello-world application 
for my foyer into Win32 programming... A literal handfull of "lucky" 
individuals who play a nearly 20 year old game for the nostalgia, rejoice.

Here I am, August 3rd 2025 - and this is not hyperbole - on an remote
uninhabited island in coastal BC, building apps with an off-grid solar-powered 
hermits haven, and spite. I suppose it's time to start versioning this oddity.

## NOTE:

Currently this utility only supports the Collectors Edition, however it would
be trivial to add support for other versions, Just need the hex offsets, and
test.  I only have the Collectors Edition here to test with.

## Building

There are no dependencies. In order to build it you will need the following
tool(chain). 
- `gcc`
- `mingw{32,64}-{gcc,windres,crt}`
- `make`

If you want to test your win32 biniries and you don't have a VM with Windows
installed, you may also want `wine`. 

`cd` into the project dir, and simply run:
`make`

Poof! `time make` returns 0.994s to build everything. I'm not 'flexing' on 
you - as the kids say - my system is average... Although `time make -j$(nproc)`
yields 0.553s :).

## Weirdness

If you're looking at the build "system" (a flat hand hacked Makefile), and 
wondering whats going on, well... I have - for over 27 years - used Linux as 
my daily driver, thus after finding an old copy of Dungeon Lords Collectors 
Edition in one of my 55-gallon drums, I decided to install it using Wine.

Wine runs the game perfectly, then, after realizing I didn't really have time
to actually finish the game, I figured, I'd just edit the save file by "hand" 
using a hex editor. But editing values in hex is annoying, so I wrote a short 
~30-line C program to edit it in real-human readable decimal values...  

It quickly became very capable, and I added command line options, and the 
ability to ACTUALLY continue the game after completing the story. By the way, 
that was a slap in the face, the lovingly buggy game litterally tells you after
completing the story that you can  continue with your character, then 
completely ghosts you. I simply couldn't let that stand without addressing it.  

Anyhoo, It's now too useful (in the context of this obscure game) not to share
it with any other nuts still playing it. However they're likely using Windows 
right?  So, this project is compiled on a Linux system using mingw32/64 to
target Windows.  There IS an in-progress Win32API GUI, but honestly, it's more 
of a plaything as I learn the Win32API. And yes... Win32 C API, you saw that
correctly.  Why, because this is a stupid-simple tool, that needs to work on 
systems from Windows98 to Windows 11. Using the Win32 API means that there are 
zero dependencies, no fluf, no bloat, just lean and mean C code. Thanks to common-controls it's even styled to match shiny new versions of Winblows. It
compiles to < 400k (with all resources including a fancy icon), and loads/runs 
in the literal blink of an eye.

### Takeaways
- Built with indifference on Linux, and cross-compiled for Windows using 
  MinGW32/64.
- Written in pure C, no .NET, no 1GB of NodeJS dependencies (simply for your 
  favorite isEven() function).
- Allows you to munchkin your character
- Allows you to actually continue your game after story completion.
- Allows you to play a lovingly buggy old game, in a way that remains fun.


### Interactive Mode help
This help text is also displayed when running dlse with no options.

```text
Usage dlse [OPTIONS] <save file path>
See: dlse --help for more information.

 Dungeon Lords Save Editor
 (c) 2025 Tyler Anderson - <stolenlan@gmail.com>
 Bitcoin: bc1qmgv9thga0ff0le4hg6kaf6y272g5j7gm3jw6hq
          May help with the GUI :)

 Interactive mode commands:
 --------------------------
 help            Show This help
 quit            Exit the program.
 show(games)     Show available save games (default install path).

 load <save file/index>
   In order to actaully view, or do anything useful, you must load a save
   file to edit. If your game is installed in a standard location, a listing
   of the available save games will be printed after this help, and in that
   case, you can simply call load with the index number eg. load 01

 list
   List the character information (stats/skills) for the currently loaded
   save file. NOTE: the KEY column is needed for use with the set-<key> cmd.

 set-<key> <value>   (See the list of properties above for the <key>s)
   Set the given property for the loaded save file. for example, to set the
   'Diabolic Iron Will' skill to 22, you would call: set-sdir 22.

 cont(inue) <save file/index>
   With this command, you can actually continue after completing the story
   (sort of). Using this you will be able to start a new character, then
   move all stats, skills, and eqipment, to the said new character. Then
   re-play the entire story with all the gear you know and love. This
   includes all keys, and quest items. I've play tested it, and it works
   great. If you do it right, you can play through with all the relics.

   Here is the process to "continue" with your end-game character:
    1) Play through to some point in the story, ideally to the last sequence
       Just after killing the soul-eater, but BEFORE releaseing Gauldryn
       and save the game to the last save slot (10) (recommended).
    2) After saving, finish the game (release low-budget Gandalf), 
       and enjoy the end sequence. It's at this point you are lied to,
       you're told you can continue with this character... But you can't.
    3) Rage quit.
    4) Start a new game/character, as soon as you load into the new
       game, save, to a DIFFERENT slot, slot 1 (recommended).
    5) Quit the game, and run this utility. Load the NEW game in (slot 1)
       To be clear, you are loading the new save, once loaded run: cont 10
       This will copy the stats/skills/items from the last slot (10) into
       the new game in slot 1. You may have to unequip/reequip your weapons.

 NOTES: It would behoove you to make a backup of your game save before any
 shenanigans with this utiltiy. Just saying, not my fault if you get things
 backwards and overwrite your god-tier character with a level one putz.
```