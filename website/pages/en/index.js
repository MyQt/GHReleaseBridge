/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

const React = require('react');

const CompLibrary = require('../../core/CompLibrary.js');
const MarkdownBlock = CompLibrary.MarkdownBlock; /* Used to read markdown */
const Container = CompLibrary.Container;
const GridBlock = CompLibrary.GridBlock;

const siteConfig = require(process.cwd() + '/siteConfig.js');

class Button extends React.Component {
  render() {
    return (
      <div className="pluginWrapper buttonWrapper" id={this.props.key}>
        <a className="button" href={this.props.href} target={this.props.target}>
          {this.props.children}
        </a>
      </div>
    );
  }
}

Button.defaultProps = {
  target: '_self',
};

class HomeSplash extends React.Component {
  render() {
    return (
      <div className="homeContainer">
        <div className="homeSplashFade">
          <div className="wrapper homeWrapper">
            <div className="projectLogo">
              <img src={siteConfig.baseUrl + 'img/GHReleaseBridge.png'} />
            </div>
            <div className="inner">
              <h2 className="projectTitle">
                {siteConfig.title}
                <small>{siteConfig.tagline}</small>
              </h2>
              <div className="section promoSection">
                <div className="promoRow">
                  <div className="pluginRowBlock">
                    <Button
                      href={
                        siteConfig.baseUrl +
                        'docs/' +
                        'Installation.html'
                      }>
                      Get Started
                    </Button>
                    <Button
                      href={
                        siteConfig.baseUrl +
                        'docs/' +
                        'GHReleaseBridgeErrorCodes.html'
                      }>
                      API Reference
                    </Button>
	            <Button href={'https://github.com/antony-jr/GHReleaseBridge'}>
	    	      View on Github
	    	    </Button>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

class Index extends React.Component {
  render() {
    let language = this.props.language || 'en';
    const showcase = siteConfig.users
      .filter(user => {
        return user.pinned;
      })
      .map(user => {
        return (
          <a href={user.infoLink}>
            <img src={user.image} title={user.caption} />
          </a>
        );
      });

    return (
      <div>
        <HomeSplash language={language} />
        <div className="mainContainer">
          <Container padding={['bottom', 'top']}>
            <GridBlock
              align="center"
              contents={[
                {
                  content: 'GHReleaseBridge fully supports Qt Projects by flowing with its event loop and'+
			   ' thus you will have no problem integrating GHReleaseBridge with your Qt Project.',
                  image: siteConfig.baseUrl + 'img/qt.png',
                  imageAlign: 'top',
                  title: 'Supports Qt\'s Event Loop.',
                },
		{
                  content: 'Whether you are Updating or checking for Updates you '+
                           'just need to execute three slots and thats it , your package is into the future ' +
                           'and thus makes GHReleaseBridge more easier to use.',
                  image: siteConfig.baseUrl + 'img/easy.png',
                  imageAlign: 'top',
                  title: 'Simple API Interface',
                },
		{
                  content: 'GHReleaseBridge supports json for configuration files and so it is easier for you to create '+
                           'configuration files for humans.',
                  image: siteConfig.baseUrl + 'img/json.png',
                  imageAlign: 'top',
                  title: 'Json Configuration!',
                },
		{
                  content: 'GHReleaseBridge does not like leaving heavy traces for the users and thus '+
                           'it is packed with all its amazing features in a single C++ header file , this ' +
                           'makes GHReleaseBridge easier to integrate with your project.',
                  image: siteConfig.baseUrl + 'img/header.png',
                  imageAlign: 'top',
                  title: 'Header only library!',
                },
		{
                  content: '<center>This project is rated by <b>Codacy</b> with a <b>A Project Certification</b><br>' +
                           ' and thus integrating this header will not affect your source , So this project is also<br>'+
                           ' best suited for those who care about code taste.<br></center>',
                  image: siteConfig.baseUrl + 'img/clean_code.png',
                  imageAlign: 'top',
                  title: '<center>Clean C++ API.</center>',
                },

              ]}
              layout="fourColumn"
            />
          </Container>

          <Container padding={['bottom', 'top']} background="dark">
            <GridBlock
              contents={[
                {
                  content:
		 	'Have you ever wanted to AutoUpdate your app when you just make a new release on github ? Well'+
			', GHReleaseBridge just does that! This Project was created for the same purpose as QInstallerBridge '+
			', This small header simply Download\'s the latest release from your configured repo and updates them '+
			'Automatically to all users who have installed your app, writen in C++ using Qt5. '+
			'Best suitable for your Qt Projects which requires Autoupdate on new release!',
		 image: siteConfig.baseUrl + 'img/github.png',
                 imageAlign: 'right',
		 title: 'Why GHReleaseBridge ?',
                },
              ]}
            />
          </Container>

          <div className="productShowcaseSection paddingBottom">
            <h2>{"Who's Using This?"}</h2>
            <p>This project is used by all these people</p>
            <div className="logos">{showcase}</div>
            <div className="more-users">
              <a
                className="button"
                href={
                  siteConfig.baseUrl + this.props.language + '/' + 'users.html'
                }>
                More {siteConfig.title} Users
              </a>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

module.exports = Index;
