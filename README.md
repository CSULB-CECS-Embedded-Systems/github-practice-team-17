# ClassroomTest - Group GitHub Assignment 🚀

Welcome to your first GitHub assignment! This is a group project where you'll learn the basics of using Git and GitHub.

## What is GitHub? 🤔

GitHub is a platform where people store and share code. Think of it like Google Docs, but for code! It helps teams work together on projects without overwriting each other's work.

**Git** = The tool that tracks changes to your files  
**GitHub** = The website where you store your code online

---

## Assignment Goal 🎯

Each person in your group will:
1. Add your name to the Team Roster below
2. Share your favorite Bay Area sports team
3. Push (save) your changes to GitHub

**Important:** Follow the steps carefully to avoid merge conflicts!

---

## Setup Instructions 🛠️

### Step 1: Install Git

**Mac:**
1. Open Terminal (press Cmd + Space, type "Terminal")
2. Type: `git --version`
3. If Git is not installed, it will prompt you to install it

**Windows:**
1. Download Git from: https://git-scm.com/download/win
2. Run the installer (use default settings)
3. Open "Git Bash" from the Start menu

**Linux:**
```bash
sudo apt-get install git
```

### Step 2: Configure Git (First Time Only)

Open your terminal/Git Bash and type:
```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

---

## Working on the Assignment 📝

### Step 3: Clone the Repository

1. Copy the repository URL from GitHub (green "Code" button)
2. Open your terminal/Git Bash
3. Navigate to where you want to save the project:
   ```bash
   cd Desktop
   ```
4. Clone the repository:
   ```bash
   git clone [PASTE-THE-URL-HERE]
   ```
5. Go into the project folder:
   ```bash
   cd ClassroomTest
   ```

### Step 4: Add Your Information

1. Open the README.md file in a text editor (Notepad, TextEdit, VS Code, etc.)
2. Find the "Team Roster" section below
3. Add YOUR information on a NEW line following the format shown
4. Save the file

### Step 5: Avoiding Merge Conflicts ⚠️

**This is SUPER important!** Before you make changes:

1. Always PULL first to get the latest changes:
   ```bash
   git pull origin main
   ```
2. Make your changes
3. Then PUSH your changes

### Step 6: Push Your Changes to GitHub

1. Check what files you changed:
   ```bash
   git status
   ```

2. Add your changes to staging:
   ```bash
   git add README.md
   ```

3. Commit with a message:
   ```bash
   git commit -m "Added [Your Name] to team roster"
   ```

4. Pull the latest changes (in case someone pushed while you were working):
   ```bash
   git pull origin main
   ```

5. Push your changes:
   ```bash
   git push origin main
   ```

### Step 7: Verify on GitHub

Go to the GitHub repository page and check that your changes appear!

---

## Team Roster 👥

Add your name and favorite Bay Area sports team below. Each person should add ONE line.

**Format:** `- [Your Name] - Favorite Team: [Team Name]`

**Example:**
- Taylor Smith - Favorite Team: Golden State Warriors

### Add Your Name Below This Line:
<!-- Students: Add your name on a new line below. DO NOT delete other students' names! -->

- 

---

## Bay Area Sports Teams Reference 🏆

Not sure which Bay Area teams exist? Here are some popular ones:

**Basketball:**
- Golden State Warriors

**Baseball:**
- San Francisco Giants
- Oakland Athletics (A's)

**Football:**
- San Francisco 49ers
- Las Vegas Raiders (formerly Oakland Raiders)

**Hockey:**
- San Jose Sharks

**Soccer:**
- San Jose Earthquakes

---

## Quick Git Command Reference 📚

| Command | What it does |
|---------|-------------|
| `git clone [url]` | Download a repository to your computer |
| `git status` | Check what files you've changed |
| `git pull origin main` | Download latest changes from GitHub |
| `git add [filename]` | Stage a file to be committed |
| `git commit -m "message"` | Save your changes with a description |
| `git push origin main` | Upload your changes to GitHub |

---

## Troubleshooting 🔧

**Problem: "Permission denied"**
- Make sure you have access to the repository. Ask your teacher if you're not a collaborator.

**Problem: "Merge conflict"**
- This happens when two people edited the same line. Don't panic!
- Ask your teacher for help, or:
  1. Open the file with the conflict
  2. Look for markers like `<<<<<<`, `======`, `>>>>>>`
  3. Keep the changes you want, delete the markers
  4. Save, commit, and push

**Problem: "No changes detected"**
- Make sure you saved the README.md file after editing
- Make sure you're in the right folder (use `pwd` to check)

**Problem: "Authentication failed"**
- You might need to set up a Personal Access Token
- Ask your teacher for help with this

---

## Tips for Success ✨

1. **Communicate with your team** - Let them know when you're about to push changes
2. **Pull before you edit** - Always get the latest version first
3. **Push right away** - Don't wait too long after making changes
4. **One line per person** - This prevents conflicts
5. **Check GitHub** - Verify your changes are there!

---

## Need Help? 🆘

If you get stuck:
1. Read the error message carefully
2. Check the Troubleshooting section above
3. Ask your teammates
4. Ask your teacher

---

**Good luck, and welcome to the world of version control! 🎉**
