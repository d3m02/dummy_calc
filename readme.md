# Dummy calc (v0.1) 
“Simple task” for test task, I toughed. General idea – user input some equation, when enter or ‘=’ pressed – program simplify equation, until only in equation left only one number. According to operation priority – we found operation char with highest priority, trim number on the left and number on the right and make calculation…

> But everything become a nightmare when brackets appear. 
![This is an meme](meme1.png)

Key problem is that brackets can have multiple levels of depth, placing in one level.
We can’t just take first bracket and last bracket:
For example, we have 1*(3+4)-(5+6). If we take first ‘(‘ and last ‘)’, we grab (3+4)-(5+6), so we break operation priority rules.

Also, we can’t take first ‘(‘ and first ‘)’:
 1+(2+(3+4)*5) – we grab (2+(3+4), unbalanced brackets.
#
## My solution is following:
* Check – is in query any ‘(‘ and where is it. If we still have brackets:
    1. Set counter for depth level to 0, maximal depth to 0, initialize empty vector of pair <size_t, size_t> - in this vector we will store location on opening bracket and closing bracket.
    2. If we found ‘(‘ – depth_level++, check – is it new max level of depth or not. If it is – clear vector with brackets location. If we on the max level of depth – we add in vector new element and save position of opening bracket.
If we found ‘)’ – depth_ depth_level --, if we on max level of depth – add position of closing bracket to the last element of vector.
    3. If depth_level equal 0 – break, we don’t want to “explore” another brackets.
    4. Until vector with brackets positions not empty – trim from the beginning equations in brackets, call Calc::solve, receive result, check – maybe before brackets “sqrt” operation – calculate sqrt (n) and replace result, if not – replace only (n) with n. Remove first element and change position of next brackets (since we replace some equation with calculation result, positions of next brackets can be changed).
    5. When vector become empty, check again, maybe in equation still exist any ‘(‘, case we reduce only one level on depth. Repeat until we didn’t remove all brackets. 
#
At the current stage implemented following operations:
- sqrt
- raise to power
- multiplication and division 
- addition and subtraction
- and brackets.

Also, I try implement “user-friendly” interface. If user input incorrect character – this character will me colored with red and with small error message nicely hinting to erase the given character with backspace. Also input function check, is ‘sqrt’ entered correctly. “Spell-checking” not so strict, user can use spacebar, in floating numbers ‘,’ instead ‘.’, for division use both ‘\’ and ‘/’, pressing enter will be replaced with ‘=’. But it’s a little bit laggy and arrows are not available since I read char by char without echo and output on screen “valid” query.
For debugging and (maybe) feature testing purpose – it’s possible to input query with string, For both methods of inputting applies validation function, checking brackets balance and incorrect symbols input. 

#
Maybe I continue developing this project, since still have some plans:
* Add test
* Make some optimization (definitely in some places better to use links instead pointers, maybe reduce function calls amount\amount of variable\size of variables)
* Improve interface, maybe. At least – find solution, how to add cursor position changing
* Add more operations?
* Explore some features of MarkDown and CMake.. 

