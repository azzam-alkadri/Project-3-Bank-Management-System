# 🏦 Bank Clients Management System (C++)

# 🏦 [ Bank 1 ]

## 📌 Overview
A console-based banking management system built using C++.  
The system manages client records and financial transactions with persistent file-based storage.

---

## 🚀 Features
- Full CRUD Operations (Add, List, Update, Delete, Find)
- Duplicate account number prevention
- Structured multi-level menu navigation
- Formatted console table output

---

## ⚙ Technical Concepts Used
- File Handling using `fstream`
- Struct-based data modeling
- Vector data management
- Enum-based menu control
- Custom delimiter-based data parsing
- Modular function design

---

## ⚠ Challenges
- Safely updating client data inside a text file without losing records
- Keeping in-memory data synchronized with file storage


---

## 🛠 Solutions
- Implemented full file rewrite logic after update/delete operations
- Reloaded vector data after each modification
- Added validation checks before executing transactions

---

## 🎯 What I Learned
- Deeper understanding of file handling and persistent storage
- Stronger modular programming practices
- Better control of system flow and state management
- Practical implementation of safe transaction logic

---

## 💼 Project Purpose
This project simulates the core backend logic of a small banking system and demonstrates structured system design using C++.
