# Bitcoin-Clipper
This project was made as a mandatory component for the completion of my course, Fundamentals of Malware Analysis (CY3004). It demonstrates my understanding of how malwares work and the persistence techniques they use.

## Disclaimer
**This project is strictly for educational and research purposes. The intent is to better understand the mechanisms and behaviors of malware, specifically focusing on Bitcoin clipper malware. It is not intended for any malicious activities or to be used for any illegal purposes. By accessing and using the content within this repository, you agree to abide by all applicable laws and ethical standards. The author is not responsible for any misuse of the information provided here.**

## Description
Bitcoin clipper malware is a type of malicious software designed to target cryptocurrency transactions, specifically those involving Bitcoin. The primary goal of Bitcoin clipper malware is to modify or replace the recipient's cryptocurrency address in the user's clipboard with an address controlled by the attacker. As a result, when the user pastes the intended recipient's address during a transaction, they unknowingly send the cryptocurrency to the attacker's address.

## Working
The malware employs a Windows application that actively monitors clipboard activities, dynamically altering its contents whenever it detects the presence of a cryptocurrency wallet address. This manipulation is facilitated through the utilization of the Windows API and the C++ filesystem library, showcasing a sophisticated set of functionalities within the code. Here is a breakdown of the main functionalities:

**1) Persistence:** The persistence function attempts to add the executable (btc_clipper.exe) to the Windows registry in the ”Run” key. This ensures that the program is executed every time the user logs in.

**2) Clipboard Functions:**

  **i. GetClipboard:** Retrieves the current content of the clipboard.
  
  **ii. SetClipboard:** Sets the clipboard content to a predefined Bitcoin wallet address (btc).
  
  **iii. Wallet Address Validation:** The IsWallet function uses a regular expression to check whether a
  given string is a valid Bitcoin wallet address. It matches addresses that start with ’1’, ’3’, or ’bc1’,
  followed by 23 to 59 alphanumeric characters.
  
  **iv. Clipboard Update Listener:** The ClipboardFormatListenerProc function is a callback function
  that gets called when the clipboard content changes (WMCLIPBOARDUPDATE). When a clipboard
  update is detected, it checks if the new clipboard content is a valid Bitcoin wallet address. If it is, and
  it’s different from the predefined address (btc), it modifies the clipboard content to the predefined wallet
  address using SetClipboard.

**3) Main Function:** It sets up the window class (WNDCLASSW) for the clipboard listener. Registers the
window class. Creates a hidden window and adds it as a clipboard format listener. Enters a message loop
(GetMessage) to handle messages until the program is closed. Upon closing, it removes the clipboard
format listener, destroys the window, and unregisters the window class.

## Installation
1. Set up a virtual environment (optional but recommended)
2. Clone this repository
3. Navigate to the project directory
4. Run the malware in the virtual environment. The working of the malware is shown in the animation below

![Demo](https://github.com/0xFirestorm/Bitcoin-Clipper/assets/123823145/bd93568b-9c13-4a9f-85f1-ac518b31f1c4)
