# listlearner single page browser app

# purpose
* list questions from a text file and show answers by pressing the spacebar
* sort lists from less known to well known by using the left and right arrow keys

example use case: learning vocabulary

![screenshot](other/screenshot.png?raw=true)

# features
* the files only have the lines resorted and stay plaintext. the files can be viewed anywhere where text files can be viewed
* files for listlearner contain lines with space delimited columns. files can be created by hand, with libreoffice calc (use the filename suffic .csv) or excel
* the first column is the question, the rest the answer
* the filename suffix is irrelevant. it can be .txt
* columns that include spaces are surrounded by doublequotes. doublequotes inside are escaped by doubling, ""
* file changes are saved automatically after 20 seconds and on quit when using ctrl+q
* csv headers are not handled and will appear like any other line
* study time is tracked for each file, displayed in the top right corner and updated every 100 seconds

compared to spaced repetition flashcard software, with listlearner it is always possible to move freely between all questions and do immediate repeated reviews.

make backups of your files before using the app.

# example file content
~~~
aberration "a state or condition markedly different from the norm"
acquiesce "agree or express agreement"
alacrity "liveliness and eagerness"
~~~

there can be more than two columns. the first column is the question.

# keyboard controls
| key | action |
| --- | --- |
| space  | show/hide answer |
| ctrl + left arrow | move the line to the top |
| ctrl + right arrow | move the line to the bottom |
| ctrl + q | save and quit |

# how to open files
* when the app is started, via the "open" button in the top left
* by passing the path to the program when starting it, ``npm start mypath``

# tips
one way to use listlearner is to go through the list and move difficult items to the top. only well known items are moved to the bottom. this way, over time, a personalised list sorted by difficulty emerges.

# development
## requirements and setup
* install [node.js](https://nodejs.org/en/) if not already installed
* on a command-line, change into the application directory and execute ``npm install`` to install the required node modules

# licence
gpl3+

# developer information
* the csv delimiter can be changed at the top of main.js
