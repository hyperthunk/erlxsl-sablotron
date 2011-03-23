# ErlXSL Sablotron Provider

This project implements an *XSLT Engine Provider* for the [ErlXSL](http://github.com/hyperthunk/erlxsl) XSLT driver.

## Overview

The [ErlXSL](http://github.com/hyperthunk/erlxsl) OTP application provides a binding to native XSLT engines, via either a port program or linked-in port driver interface. The choice of XSLT engine is `pluggable` via a simple API, and this project implements that API in order to provide access to [Sablotron XSLT Engine](http://github.com/hyperthunk/Sablotron) via the linked-in driver interface. A port driver version may be provided at a later stage.

## Status & Release Management

This project is in alpha at the moment, and as such both the API and functionality is subject to change. We use [Semantic Versioning](http://semver.org) to manage our releases (as does [ErlXSL](http://github.com/hyperthunk/erlxsl)). All major and minor versions will be tagged for release. Release candidates (i.e., revision builds) might be tagged.

## Target System Requirements

The [ErlXSL](http://github.com/hyperthunk/erlxsl) OTP application running on a version of the Erlang/OTP platform >= R13B01, and a valid install of [Sablotron](http://github.com/hyperthunk/Sablotron) are the only software prerequisites. [Sablotron](http://github.com/hyperthunk/Sablotron) provides a number of optional features that depend on other libraries (such as iconv and the Mozilla Javascript Runtime). You should refer to its documentation if you wish to use these.

## Supported Platforms

[ErlXSL](http://github.com/hyperthunk/erlxsl) has (currently) been tested on OS-X, Debian Linux and several other debian-based distros (including Ubuntu). For a complete list of supported Operating Systems, see the [ErlXSL Wiki](http://github.com/hyperthunk/erlxsl/wiki).

The list of supported platforms for [Sablotron](http://github.com/hyperthunk/Sablotron) can be found on the project home page.
