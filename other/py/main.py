#!/usr/bin/env python
import keyboard
from threading import Thread

keys = [
    "up",    "↑", 1, 0, "w",
    "left",  "←", 0, 1, "a",
    "down",  "↓", 1, 1, "s",
    "right", "→", 2, 1, "d",
    "jump",  "c", 3, 0, "space",
    "dash",  "x", 3, 1, "j",
    "grab",  "z", 4, 0, "k",
    "demo",  "b", 4, 1, "l"
]

global el
el = []

def callback(e):
    global el
    if e.name == "enter": return;
    print(keys.index(e.name) // 5)
    if (e.event_type == "down"):
        el[keys.index(e.name) // 5].configure(bg="gray")
    else:
        el[keys.index(e.name) // 5].configure(bg="white")
        
for i in range(4, len(keys), 5):
    keyboard.on_press_key  (keys[i], callback)
    keyboard.on_release_key(keys[i], callback)

def tkmain():
    global el
    import tkinter as tk
    root = tk.Tk()
    root.wait_visibility(root)
    root.config(bg='gray')
    
    #root.option_add('*Font', 0)
    for i in range(0, len(keys), 5):
        t = tk.Label(text = keys[i + 1], width = 4, height = 2, bd = 1, bg = "white", relief = "solid", font = "monospace 3")
        t.grid(column = keys[i + 2], row = keys[i + 3], padx = 2, pady = 2)
        el.append(t)
    root.mainloop()

Thread(target = tkmain).start();
