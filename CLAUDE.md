# Claude Code Pointer

Read `AGENTS.md` and treat it as the canonical repository-wide instructions.

# gram project rules (XMPP backend on the Telegram Desktop UI)

These rules come from the project owner and override convenience:

- **UI rule:** never introduce styles, icons, colors, fonts, widgets or layouts that do not already exist in Telegram Desktop. New screens must be composed only from existing `lib_ui` widgets and existing style definitions (`st::intro*`, `st::default*`, etc.) and existing icons. Do not add new entries to `.style` files, new image/icon resources, or custom painting. If a feature has no existing Telegram UI to host it, stop and ask instead of inventing one.
- New user-visible strings are allowed only as new keys in `Telegram/Resources/langs/lang.strings`; reuse existing keys whenever the meaning matches.
- Keep XMPP code isolated in `Telegram/SourceFiles/xmpp/` and keep edits to upstream files minimal, so upstream merges stay easy.
- Talk to the project owner in Arabic.
