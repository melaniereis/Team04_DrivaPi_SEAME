# Daily Log Instructions

When asked to create or update daily logs:

1. **Location**: Store logs in `docs/standups/YYYY-MM-DD.md`
2. **Format**: Use ISO date format (YYYY-MM-DD) for filenames
3. **Day Counter**: Track day number in header (check previous logs to increment)
4. **Team Members**: Bernardo, Gaspar, Hugo, Melanie, Miguel

5. **Required Sections**:
   - Header with Day #, Date (Weekday, Month DD, YYYY), Team
   - "What We Did Today" - 2-3 sentence overview
   - "Team Progress" - Per person with ✅ Done and 🔄 In progress items
   - "Hardware" - Physical work with optional images
   - "Software" - Progress with ✅/🔄 status indicators
   - "Challenges" - Format: Problem, Who, Impact (High/Medium/Low), Solution
   - "Decisions" - Important technical choices made
   - "Standards & Research" - Relevant standards/research work
   - Navigation footer with Previous/Next links

6. **Team Roles**:
   - Bernardo: Hardware Integration & Testing
   - Gaspar: OS & Development Environment
   - Hugo: Hardware & Fabrication
   - Melanie: GUI & Team Coordination
   - Miguel: GitHub Project & Agile/Scrum

7. **Status Indicators**:
   - ✅ Completed items
   - 🔄 In progress items

8. **Images**: Use `![Description](../photos/filename.jpeg)` or HTML `<img>` tags

9. **Use current date** unless user specifies otherwise
10. **Reference template** at `docs/standups/daily-log-template.md` for exact format
11. **Check Git Activity**: Before creating the log, review:
    - Commits from that day using `git log --since="YYYY-MM-DD 00:00" --until="YYYY-MM-DD 23:59" --oneline --all`
    - Pull requests from that day (merged, opened, or closed)
    - Use this information to accurately populate Team Progress and Software sections
