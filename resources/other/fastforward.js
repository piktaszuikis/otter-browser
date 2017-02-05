(function(options, hrefTokens, classTokens, idTokens, textTokens){

	function parseUrl(link){
		var result = {
			pathname: link.pathname.split('/'),
			query: []
		};

		if(link.search) {
			var query = link.search.substr(1).split('&');
			query.every(function(item) {
				result.query.push(item.split('=')[0]);
			});
		}

		return result;
	}

	function join() {
		result = ''
		for(var i = arguments.length - 1; i >= 0; i--) {
			result += arguments[i] + ' ';
		}
		
		return result;
	}
	
	function calculateScore(value, tokens) {
		var score = 0;

		if(value) {
			for(var i = tokens.length - 1; i >= 0; i--) {
				if(value.indexOf(tokens[i].value) != -1) {
					score += tokens[i].score;
				}
			}
		}

		return score;
	}
	
	function calculateScoreForValues(values, tokens) {
		var score = 0;

		if(values && values.length > 0) {
			for(var i = values.length - 1; i >= 0; i--) {
				score += calculateScore(values[i].toUpperCase(), tokens);
			}
		}

		return score;
	}
	
	function getFastForward(isAny) {
		var links = document.querySelectorAll('a:not([href^="javascript:"]):not([href="#"])')
		var scoredLinks = [];
		
		if(options.debug)
			console.log("FastForward DEBUG: Checking", links.length, "links");

		for(var i = links.length - 1; i >= 0; i--){
			var score = 0;
			var link = links[i];
			
			//check for rel
			if(link.rel && link.rel.indexOf('next') != -1)
				score += options.relScore;

			//check for innerText, aria-label and alt
			var linkText = join(link.innerText, link.getAttribute('aria-label'), link.getAttribute('alt'), link.title).toUpperCase();
			score += calculateScore(linkText, textTokens);

			//check for id
			score += calculateScore(link.id.toUpperCase(), idTokens);

			//check for class
			score += calculateScoreForValues(link.classList, classTokens);

			//check for location/href
			var url = parseUrl(link);
			score += calculateScoreForValues(url.pathname, hrefTokens)
			score += calculateScoreForValues(url.query, hrefTokens)

			//Is link worthy?
			if(score > options.threshold){
				if(isAny) {
					if(options.debug)
						console.log("FastForward DEBUG: Found at least one link for FastForward. Score:", score);
					return link.href;
				} else {
					scoredLinks.push({score: score, link: link});
				}
				
			}
		}

		if(scoredLinks.length > 0) {
			scoredLinks.sort(function(a, b){return b.score - a.score});

			if(options.debug) {
				for(var i = 0; i < scoredLinks.length; i++)
					console.log("FastForward DEBUG: ", scoredLinks[i].score, "Url:", scoredLinks[i].link.outerHTML);
				
				console.log("FastForward DEBUG: Choosing link with score", scoredLinks[0].score, scoredLinks[0].link.href);
			}

			return scoredLinks[0].link.href;
		} else if(options.debug) {
			console.log("FastForward DEBUG: No candidate links found!");
		}
		
		return null;
	}
	
	
	return {
		hasFastForward : function() { return !!getFastForward(false) },
		getFastForward : function() { return getFastForward(false) }
	}
	
})(%options%, %hrefTokens%, %classTokens%, %idTokens%, %textTokens%)
