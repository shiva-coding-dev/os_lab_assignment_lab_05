#!/usr/bin/env python3
"""Render the checked-in xv6 console transcripts as terminal-style PNGs."""
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "screenshots"
OUT.mkdir(exist_ok=True)

try:
    font = ImageFont.truetype("/System/Library/Fonts/Menlo.ttc", 22)
    small = ImageFont.truetype("/System/Library/Fonts/Menlo.ttc", 16)
except OSError:
    font = ImageFont.load_default()
    small = font

for log in sorted((ROOT / "logs").glob("*.log")):
    lines = log.read_text().splitlines()
    width = 1280
    line_height = 32 if font != ImageFont.load_default() else 16
    height = max(180, 100 + line_height * len(lines))
    image = Image.new("RGB", (width, height), "#101418")
    draw = ImageDraw.Draw(image)
    draw.rectangle((0, 0, width, 54), fill="#242a31")
    draw.ellipse((22, 19, 36, 33), fill="#ff5f57")
    draw.ellipse((46, 19, 60, 33), fill="#febc2e")
    draw.ellipse((70, 19, 84, 33), fill="#28c840")
    draw.text((110, 17), "xv6 console transcript", font=small, fill="#c9d1d9")
    y = 78
    for line in lines:
        draw.text((34, y), line, font=font, fill="#d7f9d0")
        y += line_height
    output = OUT / (log.stem + ".png")
    image.save(output)
    print(output)
