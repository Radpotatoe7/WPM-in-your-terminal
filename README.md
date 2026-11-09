# WPM-in-your-terminal
A simple C program which runs a WPM test in your terminal , with difficulty levels , and custom text support.
Note: **ONLY WORKS IN LINUX.** This C program uses libraries and input functions that only work in linux. This will NOT work in windows

# How to run: (LINUX ONLY)
(0. If you are new to github , you usually download the files by pressing the big green code button on the top right of the index of contents , then download it as zip and extract it)
1. Download/Extract the wpm folder , which contains the c file and the texts folder (source for the displayed text).
2. Open your terminal , cd into the wpm folder , and run `gcc wpm.c -o wpm` to create an executable from the C source code.
3. Run `./wpm` in your terminal
4. The app should be running now.

# Features:
## Difficulty Choice

<img width="531" height="32" alt="screenshot_2026-11-09_16-04-09" src="https://github.com/user-attachments/assets/9441482b-6b6d-44dc-838b-eb5bf520ede5" />

<img width="239" height="175" alt="screenshot_2026-11-09_16-04-27" src="https://github.com/user-attachments/assets/1854b6e7-2dba-4e14-aa38-03c3f5519c18" />

The choice you make chooses the appropriate text from the source (texts) folder.
You can add your own custom text in these .txt files for more texts to practice with.

## 60 Second Test

<img width="268" height="52" alt="screenshot_2026-11-09_16-08-05" src="https://github.com/user-attachments/assets/fce3dfaf-a636-4457-8c0a-42f0f92b8416" />

The test will always end in 60 seconds (1 minute) , as a standard wpm test.

## Accuracy shown 

<img width="309" height="42" alt="screenshot_2026-11-09_16-08-56" src="https://github.com/user-attachments/assets/95e7a45d-f142-4acf-8edf-1f9cbe826431" />

The test will show how many words were matched with the source text, giving an accuracy check.

## Color Coded

<img width="1272" height="225" alt="screenshot_2026-11-09_16-09-56" src="https://github.com/user-attachments/assets/09f4a774-edc8-430b-af60-a4c4709ec80f" />

Colors are given for the correctly matched and wrong text.

## Random Text Choice in the Same Difficulty
Even in the same difficulty text file , the program can give a different text. This is because the program chooses 1 line from the text , and each lines in the given txt files are big enough for a 60 second wpm test.

<img width="593" height="250" alt="screenshot_2026-11-09_16-07-10" src="https://github.com/user-attachments/assets/25fefcc5-a3e4-4b97-a68f-7a279cdf3e8a" />

If you want to add your own custom text , make sure to add it as one big line , like the ones shown / given in the texts folder.


# Things to add
- Adding calculation of wpm speed
- Better looking interface
- Space Spam Prevention
- Add custom timer
