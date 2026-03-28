#!/usr/bin/env python3
"""Fetch Codeberg heatmap data and generate an SVG contribution graph."""

import json
import urllib.request
from collections import defaultdict
from datetime import datetime, timedelta

USERNAME = "bgonc"
API_URL = f"https://codeberg.org/api/v1/users/{USERNAME}/heatmap"
OUTPUT = "heatmap.svg"

CELL = 11
GAP = 2
STEP = CELL + GAP
MONTHS = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
DAYS = ["Mon", "", "Wed", "", "Fri", "", ""]

# Color scales (light theme friendly)
COLORS = ["#ebedf0", "#9be9a8", "#40c463", "#30a14e", "#216e39"]


def get_color(count):
    if count == 0: return COLORS[0]
    if count <= 2: return COLORS[1]
    if count <= 5: return COLORS[2]
    if count <= 10: return COLORS[3]
    return COLORS[4]


def main():
    # Fetch data
    with urllib.request.urlopen(API_URL) as r:
        data = json.loads(r.read())

    # Aggregate by date
    by_date = defaultdict(int)
    for entry in data:
        d = datetime.fromtimestamp(entry["timestamp"])
        key = d.strftime("%Y-%m-%d")
        by_date[key] += entry["contributions"]

    # Build year grid
    today = datetime.now()
    start = today - timedelta(days=364)
    # Align to Monday
    start -= timedelta(days=(start.weekday()))

    days = []
    cursor = start
    total = 0
    while cursor <= today:
        key = cursor.strftime("%Y-%m-%d")
        count = by_date.get(key, 0)
        days.append({"date": key, "count": count, "dow": cursor.weekday()})
        total += count
        cursor += timedelta(days=1)

    # Group into weeks
    weeks = []
    week = []
    # Pad first week
    first_dow = days[0]["dow"]
    for i in range(first_dow):
        week.append(None)
    for day in days:
        week.append(day)
        if len(week) == 7:
            weeks.append(week)
            week = []
    if week:
        while len(week) < 7:
            week.append(None)
        weeks.append(week)

    # Month labels
    month_labels = []
    last_month = -1
    for wi, w in enumerate(weeks):
        for d in w:
            if d:
                m = int(d["date"].split("-")[1]) - 1
                if m != last_month:
                    month_labels.append((MONTHS[m], wi * STEP))
                    last_month = m
                break

    left_pad = 28
    top_pad = 18
    svg_w = len(weeks) * STEP + left_pad + 10
    svg_h = 7 * STEP + top_pad + 28

    lines = []
    lines.append(f'<svg width="{svg_w}" height="{svg_h}" xmlns="http://www.w3.org/2000/svg">')
    lines.append(f'<rect width="{svg_w}" height="{svg_h}" fill="none"/>')

    # Month labels
    for label, x in month_labels:
        lines.append(f'<text x="{x + left_pad}" y="12" fill="#656d76" font-size="9" font-family="ui-monospace,monospace">{label}</text>')

    # Day labels
    for i, label in enumerate(DAYS):
        if label:
            y = top_pad + i * STEP + CELL - 2
            lines.append(f'<text x="0" y="{y}" fill="#656d76" font-size="9" font-family="ui-monospace,monospace">{label}</text>')

    # Cells
    for wi, w in enumerate(weeks):
        for di, day in enumerate(w):
            if day is None:
                continue
            x = wi * STEP + left_pad
            y = di * STEP + top_pad
            color = get_color(day["count"])
            lines.append(f'<rect x="{x}" y="{y}" width="{CELL}" height="{CELL}" rx="2" fill="{color}">'
                         f'<title>{day["count"]} contributions on {day["date"]}</title></rect>')

    # Footer
    fy = 7 * STEP + top_pad + 8
    lines.append(f'<text x="{left_pad}" y="{fy}" fill="#656d76" font-size="10" font-family="ui-monospace,monospace">{total} contributions in the last year</text>')

    # Legend
    lx = svg_w - 10 - 5 * STEP - 40
    lines.append(f'<text x="{lx - 28}" y="{fy}" fill="#656d76" font-size="9" font-family="ui-monospace,monospace">Less</text>')
    for i, c in enumerate(COLORS):
        lines.append(f'<rect x="{lx + i * STEP}" y="{fy - 9}" width="{CELL}" height="{CELL}" rx="2" fill="{c}"/>')
    lines.append(f'<text x="{lx + 5 * STEP + 2}" y="{fy}" fill="#656d76" font-size="9" font-family="ui-monospace,monospace">More</text>')

    lines.append("</svg>")

    with open(OUTPUT, "w") as f:
        f.write("\n".join(lines))

    print(f"Generated {OUTPUT} — {total} contributions, {len(weeks)} weeks")


if __name__ == "__main__":
    main()
