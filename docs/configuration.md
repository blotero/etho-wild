# Configuration

EthoWild uses a JSON configuration file (`behaviors.json`) to define the behaviors you can label, along with metadata options like roles, sexes, and stages.

## File Location

The `behaviors.json` file must be in the **same directory** as the EthoWild executable. The application loads it automatically on startup.

---

## Configuration Structure

Here's the complete structure of the configuration file:

```json
{
  "behaviors": {
    "Category Name": {
      "Behavior 1": "EVENT",
      "Behavior 2": "STATE"
    }
  },
  "roles": ["role1", "role2"],
  "sexes": ["male", "female", "undefined"],
  "stages": ["adult", "juvenile", "calf"],
  "group_types": ["individual", "group"]
}
```

---

## Events vs States

EthoWild supports two types of behaviors:

### Events

**Events** are instantaneous behaviors that occur at a single point in time.

- Recorded with a single double-click
- Have only a start time (no duration)
- Examples: breathing, diving, a single contact

```json
"Respiración": "EVENT"
```

### States

**States** are duration-based behaviors that span a period of time.

- First double-click **starts** the state
- Second double-click **ends** the state
- Records both start time and end time, plus calculated duration
- While a state is active, other behaviors are disabled
- Examples: displacement, synchronized swimming

```json
"Desplazamiento": "STATE"
```

!!! tip "Visual Feedback"
    When a state is active, the application shows a green indicator with the behavior name. All other behaviors in the tree are temporarily disabled until you end the current state.

---

## Behaviors Section

The `behaviors` object contains categories, each with their own set of behaviors.

```json
{
  "behaviors": {
    "Individual": {
      "Breathing": "EVENT",
      "Diving with tail": "EVENT",
      "Diving without tail": "EVENT",
      "Displacement": "STATE"
    },
    "Social": {
      "Synchronized breathing": "EVENT",
      "Grouping": "EVENT",
      "Synchronized displacement": "STATE",
      "Swimming in file": "STATE"
    },
    "Reproductive": {
      "Approach": "EVENT",
      "Fast approach": "EVENT",
      "Mounting": "EVENT",
      "Release": "EVENT"
    }
  }
}
```

In the application, categories appear as expandable tree nodes, with individual behaviors as child items.

---

## Metadata Options

### Roles

Roles define the subject being observed (e.g., mother, calf, escort).

```json
"roles": ["mother", "calf", "escort", "male", "female", "undefined"]
```

### Sexes

Sex options for the observed subject.

```json
"sexes": ["male", "female", "undefined"]
```

### Stages

Life stages of the observed subject.

```json
"stages": ["calf", "adult", "juvenile", "undefined"]
```

### Group Types

Whether the observation is of an individual or a group.

```json
"group_types": ["individual", "group"]
```

---

## Example Configuration

Here's a minimal example configuration:

```json
{
  "behaviors": {
    "Feeding": {
      "Foraging": "STATE",
      "Catch prey": "EVENT",
      "Swallow": "EVENT"
    },
    "Movement": {
      "Walking": "STATE",
      "Running": "STATE",
      "Jumping": "EVENT"
    },
    "Social": {
      "Grooming": "STATE",
      "Play": "STATE",
      "Aggression": "EVENT",
      "Vocalization": "EVENT"
    }
  },
  "roles": ["focal", "partner", "group_member"],
  "sexes": ["male", "female", "unknown"],
  "stages": ["infant", "juvenile", "adult", "elderly"],
  "group_types": ["solitary", "pair", "group"]
}
```

---

## Troubleshooting

!!! warning "Configuration Error"
    If the configuration file is missing or invalid, EthoWild will show a warning dialog and start with an empty configuration. You won't be able to label behaviors until you provide a valid `behaviors.json` file.

Common issues:

| Problem | Solution |
|---------|----------|
| File not found | Ensure `behaviors.json` is in the same directory as the executable |
| JSON parse error | Validate your JSON syntax (use a tool like [jsonlint.com](https://jsonlint.com)) |
| Empty behavior tree | Check that `behaviors` object is not empty |

