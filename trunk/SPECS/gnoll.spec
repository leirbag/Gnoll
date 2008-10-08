Name: gnoll		
Version: 0.1.4	
Release: 1%{?dist}
Summary: A 3D action/rpg game engine

Group: Amusements/Games	
License: GPLv2+	
URL: http://www.gnoll.org		
Source0: http://www.gnoll.org/download/%{name}-%{version}.tar.gz	
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: scons, ogre-devel, pkgconfig, boost-devel, libxml++-devel
BuildRequires: libxml2-devel, glibmm24-devel, cegui-devel, ois-devel
BuildRequires: openal-devel, freealut-devel, libvorbis-devel, libogg-devel
BuildRequires: libsndfile-devel

Requires: ogre, boost, libxml++, libxml2, glibmm24, cegui, ois
Requires: openal, freealut, libvorbis, libogg, libsndfile

%description
This is a 3D action/rpg game engine

%prep
%setup -q -n trunk


%build
scons %{?_smp_mflags} gnoll


%install
mkdir -p $RPM_BUILD_ROOT/usr
scons install prefix=$RPM_BUILD_ROOT/usr/
rm -rf $RPM_BUILD_ROOT/usr/data/*
rm -rf $RPM_BUILD_ROOT/usr/bin/launch-gnoll

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc README COPYING AUTHORS
/usr/bin/gnoll

%changelog

* Tue Oct 7 2008 Bruno Mahe <bruno[AT]gnoll.org>
- Initial build.
