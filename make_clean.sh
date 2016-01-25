#!/bin/bash

to_remove=(
		'*.ac'
		'*.cache'
		'*.in'
		'*.log'
		'*.m4'
		'*.scan'
		'Makefile'
		'compile'
		'config.h'
		'config.status'
		'configure'
		'depcomp'
		'install-sh'
		'missing'
	)

for i in "${to_remove[@]}"; do
	find -iname "$i" -exec rm -rv {} \;
done
