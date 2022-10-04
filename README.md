# jim
Minimalistic text viewer.

Features:


# Install
```
cmake .
cmake --build . --target jim
```
# Run 
```
./jim <filename>
```

# TODO:
- [X] Read text file and display content
    - [ ] Only read the currently displayed chuck of text to make reading large files quicker
- [X] Line number
- [ ] Scrolling with arrows
    - [ ] Scroll wheel support
- [ ] Handle SIGINT gracefully
- [ ] Syntax highlight
- [ ] Search function
