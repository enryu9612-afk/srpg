# Game Design Document: Roguelike SRPG

## 1. Core Concept
- **Goal**: Implement a Roguelike SRPG.
- **Language**: C
- **Engine**: raylib
- **Graphics**: Purely text-based and special characters.

## 2. World Structure
### Floor Progression
- **Cycle**: 4 Regular Floors $\rightarrow$ 1 Boss Floor $\rightarrow$ 1 Rest Floor. This cycle repeats 4 times.
- **Terminology**: One cycle (4 Regular + 1 Boss) is called a "Sector".
- **Floor Types**:
    - **Regular/Boss Floors**: Tile-based (1 character = 1 tile).
    - **Rest Floors**: Text-based interface (Shop, etc.).
- **NPCs**: Exactly one NPC appears per Sector (excluding Boss floors). NPCs provide quests and rewards.

### Rewards
- **Boss Floor**: Clearing grants one rare Relic and one piece of Equipment.
- **Rest Floor**: Access to shops.

## 3. Combat & Entities
### Common Entity Stats
All entities (Player/Enemy) possess:
- `id`, `Attack`, `Evasion`, `Accuracy`, `HP`, `Defense`, `Magic Resistance`, `Level`, `Skills`.

### Operator (Player Unit) Specifics
#### Specialized Stats (I to V)
Represented by Roman numerals. Each level increases the effect:
- **Eye (눈)**: Accuracy (+10% / +25% / +45% / +70% / +100%)
- **Ear (귀)**: Evasion (+10% / +25% / +45% / +70% / +100%)
- **Tongue (혀)**: SP Cost Reduction (-1 / -2 / -3 / -4 / -5)
- **Hand (손)**: Crit Chance & Equipment Effect (+5% / +10% / +20% / +35% / +50%)
- **Heart (심장)**: Attack & HP (+5% / +10% / +20% / +35% / +50%)

#### Growth & Leveling
- **Stat Points**: +1 point per level up.
- **Stat Upgrade Cost**: To upgrade from $n-1$ to $n$, it costs $n$ points.
- **Base Stats**: Increase by 1.2x per level.
- **Experience**: Gain EXP equal to the defeated enemy's level.
- **Level Up Req**: `Current Level * 10` EXP.
- **Level Range**: 1 (Initial) to 25 (Max).

#### Classes
- **Guide (길잡이)**: Support focus.
- **Wizard (요술쟁이)**: Variable creation/Powerful attacks.
- **Opponent (대적자)**: Powerful attack and tanking.
*(Classes are for categorization; no intrinsic unique abilities yet).*

### Damage & Accuracy Formulas
- **Physical Damage**: $\text{Current Attack} - \text{Target Defense}$
- **Magic Damage**: $\text{Current Attack} \times (1 - \frac{\text{Target Magic Resistance}}{100})$
- **Final Accuracy**: $\text{Attacker Accuracy} - \text{Defender Evasion}$

## 4. Enemies
### Enemy Types
| Type | Skills | Drop Chance | Gold | Other |
| :--- | :--- | :--- | :--- | :--- |
| **Normal** | 0 | 6% Common Equip | 10g | - |
| **Elite** | 1 | 100% Rare Equip | 10% 100g / 90% 20g | 1.5x Stats, EXP = Level * 2 |
| **Boss** | 3 | 100% Boss Equip | 200g | - |

*Elite enemies have a 5% chance to replace a Normal enemy upon spawning.*

### Enemy AI
1. If Operator is within skill range $\rightarrow$ Attack.
2. Otherwise $\rightarrow$ Use buff/heal skill OR move toward the nearest Operator.

## 5. Items & Equipment
### Item Categories
- **Equipment**: Weapons, Armor, Accessories.
- **Relics**: Global effects applied to all Operators.
- **Consumables**: SP/Attack/HP recovery and buffs.
- **Recruitment Tickets**: Recruit a random Operator (Level: $\text{Current Floor} \pm 3$).

### Rarity Tiers
- **Common (조잡, 평범)**
- **Rare (고급, 희귀)**
- **Unique (유일)**: Found only in chests.

### Equipment Details
- **Slots**: 1 Weapon, 1 Armor, 1 Accessory per Operator (can be expanded).
- **Weapons**: (Base Atk: 10/20/30/40/55 based on rarity). All base range = 1.
    - **Sword**: Attack +10%.
    - **Spear**: Range +1.
    - **Crossbow**: Range +4, Accuracy -10% per tile of distance.
    - **Staff**: Magic Damage. Bonus based on **Tongue** stat (+5% to +50%) instead of **Heart**.
    - **Mace**: Ignore 50% Defense, Attack -20%.
- **Armor**:
    - **Light**: Small Defense increase.
    - **Heavy**: High Defense increase, slight Attack decrease.
    - **Magic**: Magic Resistance increase.
- **Accessories**: Various special effects (e.g., Burn immunity, stat boosts).

### Relic Examples
- **Chicken Game**: Attack +100%, HP -99%.
- **Scope**: Crossbow Range +5, removes Accuracy penalty.
- **Adrenaline**: Attack +20%.
- **Anti-Magic Barrier**: Magic Resistance +10%.

## 6. Status Effects
- **Rage (분노)**: Attack $\uparrow$, Defense $\downarrow$.
- **Hunger (굶주림)**: Max HP $\downarrow$.
- **Sickly (병약)**: Defense $\downarrow$, Magic Resistance $\downarrow$.
- **Burn (화상)**: Lose 5% Max HP at turn end (ignores all defenses).

*Format: `Status Name Duration` (e.g., Burn 2). Duration decreases by 1 each turn.*

## 7. Battle System
- **Turn-based**: Player Turn $\rightarrow$ Enemy Turn.
- **Player Actions**: Move (Up/Down/Left/Right), Attack, Use Consumable, Change Equipment.
- **Attack Constraints**: Only within range, no diagonal attacks.
- **Skills**:
    - Operators start with 1 skill, gain more at levels 10 and 20.
    - **Example**: `Slam (강타) I/II/III` $\rightarrow$ Costs 5 SP, deals $(100 + n \times 10)\%$ damage.

## 8. First Layer Concept: Famine (기근)
- **Enemies**: Starving Farmers, Mercenaries, Knights, Hounds, Wizards.
- **Boss**: Amon.
