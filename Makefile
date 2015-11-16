CODEDIR = source

.PHONY: debug

all:
	$(MAKE) -C $(CODEDIR)

debug:
	$(MAKE) -C $(CODEDIR) debug

release:
	$(MAKE) -C $(CODEDIR) release

clean:
	$(MAKE) -C $(CODEDIR) clean

assembly:
	$(MAKE) -C $(CODEDIR) assembly

burn:
	$(MAKE) -C $(CODEDIR) burn
