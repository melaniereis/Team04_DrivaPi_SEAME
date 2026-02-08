# Daily Log Instructions

When asked to create or update daily logs:

1. **Location**: Store logs in `docs/standups/YYYY-MM-DD.md`
2. **Format**: Use ISO date format (YYYY-MM-DD) for filenames
3. **Day Counter**: Track day number in header (check previous logs to increment)
4. **Team Members**: Bernardo, Gaspar, Hugo, Melanie, Miguel

5. **Date Detection & Assignment**:
   - Identify the requesting user by their GitHub username and assigned weekday:
     - `Zahhask45` (Gaspar) → Monday logs
     - `hugofslopes` (Hugo) → Tuesday logs
     - `melaniereis` (Melanie) → Wednesday logs
     - `berestv` (Bernardo) → Thursday logs
     - `Biltes` (Miguel) → Friday logs
   - **Date Determination Logic** (if no date specified in prompt):
     1. Identify user's assigned weekday from the list above
     2. Use `date -d "last [weekday]" +%Y-%m-%d` to find the date of the last occurrence of that weekday
     3. Use that date for the new daily log
     - Example: If Gaspar (Monday) requests a daily on Wednesday 2026-02-04:
       - Run `date -d "last Monday" +%Y-%m-%d` → returns `2026-02-02`
       - Create log for `2026-02-02.md`
     - Example: If Hugo (Tuesday) requests a daily on Wednesday 2026-02-04:
       - Run `date -d "last Tuesday" +%Y-%m-%d` → returns `2026-02-03`
       - Create log for `2026-02-03.md`
     - If user explicitly specifies a date in the request, use that date regardless of their assigned day
   - Note: Any team member can create logs for another day if explicitly requested (e.g., covering for sick team member)
   - **Check Previous Log**: Always check and read the daily log from the day before the target date to:
     - Get correct day counter (increment by 1)
     - Reference ongoing work or blockers
     - Maintain continuity in navigation footer

6. **Required Sections**:
   - Header with Day #, Date (Weekday, Month DD, YYYY), Team
   - "What We Did Today" - 2-3 sentence overview
   - "Team Progress" - Per person with ✅ Done and 🔄 In progress items
   - "Hardware" - Physical work with optional images
   - "Software" - Progress with ✅/🔄 status indicators
   - "Challenges" - Format: Problem, Who, Impact (High/Medium/Low), Solution
   - "Decisions" - Important technical choices made
   - "Standards & Research" - Relevant standards/research work
   - Navigation footer with Previous/Next links

7. **Team Roles**:
   - Bernardo: Hardware Integration & Testing
   - Gaspar: OS & Development Environment
   - Hugo: Hardware & Fabrication
   - Melanie: GUI & Team Coordination
   - Miguel: GitHub Project & Agile/Scrum

8. **Status Indicators**:
   - ✅ Completed items
   - 🔄 In progress items

9. **Images**: Use `![Description](../photos/filename.jpeg)` or HTML `<img>` tags

10. **Reference template** at `docs/standups/daily-log-template.md` for exact format
11. **Check Git Activity**: Before creating the log, review:
    - Commits from that day using `git log --since="YYYY-MM-DD 00:00" --until="YYYY-MM-DD 23:59" --oneline --all`
    - Pull requests from that day (merged, opened, or closed)
    - Use this information to accurately populate Team Progress and Software sections
