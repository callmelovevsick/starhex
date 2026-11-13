CXX ?= g++
CXXFLAGS ?= -std=c++17 -O3 -Wall -Wextra -pedantic
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
DATADIR ?= $(PREFIX)/share
MANDIR ?= $(DATADIR)/man/man1

TARGET = starhex
SOURCES = src/main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	install -Dm644 completions/starhex.bash $(DESTDIR)$(DATADIR)/bash-completion/completions/$(TARGET)
	install -Dm644 completions/_starhex $(DESTDIR)$(DATADIR)/zsh/site-functions/_$(TARGET)
	install -Dm644 completions/starhex.fish $(DESTDIR)$(DATADIR)/fish/vendor_completions.d/$(TARGET).fish
	install -Dm644 doc/starhex.1 $(DESTDIR)$(MANDIR)/starhex.1

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)
	rm -f $(DESTDIR)$(DATADIR)/bash-completion/completions/$(TARGET)
	rm -f $(DESTDIR)$(DATADIR)/zsh/site-functions/_$(TARGET)
	rm -f $(DESTDIR)$(DATADIR)/fish/vendor_completions.d/$(TARGET).fish
	rm -f $(DESTDIR)$(MANDIR)/starhex.1

clean:
	rm -f $(TARGET)
	rm -rf build

.PHONY: all install uninstall clean
