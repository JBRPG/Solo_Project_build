This game can be run on any platform since it uses native C++ 2011
including but not limited to Windows, MacOSX, Linux, etc.

This build uses Windows Visual Studios 2013, which has the solution.

However, you will need to refer to the official site of SFML
to learn how to link SFML files with your C++ project


About the game:

Game title is Randamius: Infinite Survival Shooter

I made this game by myself to understand genere conventions,
procedural/random generation, general polishing, and
making a gameplay engine from scratch with C++ and SFML.

I wanted to make an endless shooter with one life to
make the experience more towards a survival-based score run.
To aid the experience, I adopted a time-based progressive difficulty,
which the enemy waves start out easy, but get harder the longer
the player survives.

Also, I have used some elements of procedural and random generation.

Enemy waves are created randomly, but each type of enemy movement and
shot type must be explored at least once before going to the next cycle.

Terrain and its land enemy are generated randomly with a fixed factor.
