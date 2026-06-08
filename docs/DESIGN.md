### Anchor
Voltagent — a developer-focused terminal aesthetic that borrows precise hairline feature-cards and an electric-green accent, adapted to support both dark canvas and a clean light mode.

### Tokens
- **Typography**: Inter (body/display) and SF Mono (data/code). Base 16px.
- **Colors**: 
  - Dark Mode: Canvas `#101010`, Primary `#00d992`, Ink `#f2f2f2`, Hairline `#3d3a39`.
  - Light Mode: Canvas `#ffffff`, Primary `#00b87a`, Ink `#101010`, Hairline `#e5e7eb`.
- **Spacing**: Base 4px. 8px (sm), 12px (md), 24px (2xl).
- **Radius**: 6px for buttons, 8px for cards.
- **Shadow**: No material shadow; 1px solid hairline borders define elevation.
- **Motion**: 160ms ease. Reduced-motion fallback: 0.01ms.

### Constraints
- WCAG 2.2 AA floor met.
- Ensure all user-supplied input is sanitized, all status indicators have non-color alternatives, and all interactive elements have visible focus states.
- Anti-patterns: No heavy material drop-shadows, no generic gradient meshes, no dashboard KPI card-walls.

### Previous Directions
- digital peak programme meter
